#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <node.h>
#include <uv.h>
#include <jni.h>
#include <string>

using namespace v8;
using namespace std;

static JavaVM *jvm = NULL;
static jobject launcher = NULL;
static jmethodID getDougService;
static jmethodID sayHi;

static JavaVM *getJVM() {
	if (!jvm) {
		// Startup JRE
		printf("Creating VM\n");
		JNIEnv *env;
		JavaVMInitArgs vm_args;
		JavaVMOption *options = new JavaVMOption[1];
		options[0].optionString = strdup("-Djava.class.path=../dougEmbedEquinox/bin:../org.eclipse.osgi/org.eclipse.osgi_3.12.0.v20170302-2050.jar");
		vm_args.version = JNI_VERSION_1_6;
		vm_args.nOptions = 1;
		vm_args.options = options;
		vm_args.ignoreUnrecognized = false;
		JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
		delete[] options;

		// Create and start the Launcher
		jclass launchClass = env->FindClass("doug/embed/Launcher");
		jmethodID constructor = env->GetMethodID(launchClass, "<init>", "()V");
		launcher = env->NewObject(launchClass, constructor);

		// cache method ids
		getDougService = env->GetMethodID(launchClass, "getDougService", "()Ldoug/embed/services/IDougService;");

		jclass dougServiceClass = env->FindClass("doug/embed/services/IDougService");
		sayHi = env->GetMethodID(dougServiceClass, "sayHi", "()Ljava/lang/String;");
	}
	return jvm;
}

struct SayHiJob {
    uv_work_t request;
    Persistent<Promise::Resolver> resolver;
    string result;
};

static void SayHi(const FunctionCallbackInfo<Value> &args) {
    auto isolate = args.GetIsolate();
    HandleScope handleScope(isolate);

    auto job = new SayHiJob();
    job->request.data = job;

    auto resolver = Promise::Resolver::New(isolate);
    job->resolver.Reset(isolate, resolver);

    auto work = [](uv_work_t *req) {
        auto job = static_cast<SayHiJob *>(req->data);
        JavaVM *jvm = getJVM();
        JNIEnv *env;
        jvm->AttachCurrentThread((void **) &env, NULL);

        jobject dougService = env->CallObjectMethod(launcher, getDougService);
        jstring hi = (jstring) env->CallObjectMethod(dougService, sayHi);
        job->result = env->GetStringUTFChars(hi, NULL);
        jvm->DetachCurrentThread();
    };

    auto after = [](uv_work_t* req, int status) {
        auto isolate = Isolate::GetCurrent();
        HandleScope handleScope(isolate);

        auto job = static_cast<SayHiJob *>(req->data);
        auto result = String::NewFromUtf8(isolate, job->result.c_str());
        auto resolver = Local<Promise::Resolver>::New(isolate, job->resolver);
        resolver->Resolve(isolate->GetCurrentContext(), result);
    
        job->resolver.Reset();
        delete job;
    };

    uv_queue_work(uv_default_loop(), &job->request, work, after);

    args.GetReturnValue().Set(resolver->GetPromise());
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "sayHi", SayHi);
}

NODE_MODULE(addon, init)
