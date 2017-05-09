package doug.embed;

import java.util.HashMap;
import java.util.Map;
import java.util.ServiceLoader;

import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.launch.Framework;
import org.osgi.framework.launch.FrameworkFactory;

import doug.embed.services.IDougService;

public class Launcher {
	
	private Framework framework;
	private IDougService dougService;

	public Launcher() throws Exception {
		FrameworkFactory frameworkFactory = ServiceLoader.load(FrameworkFactory.class).iterator().next();
		
		Map<String, String> config = new HashMap<>();
		config.put("osgi.configuration.area", "./configuration");
		config.put(Constants.FRAMEWORK_SYSTEMPACKAGES_EXTRA, "doug.embed.services");
		
		framework = frameworkFactory.newFramework(config);
		framework.start();
		
		BundleContext context = framework.getBundleContext();

		Bundle provider = context.installBundle("file:../doug.embed.provider");
		provider.start();
	}
	
	public void stop() throws Exception {
		framework.stop();
	}
	
	public static void main(String[] args) throws Exception {
		Launcher launcher = new Launcher();
		
		System.out.println(launcher.getDougService().sayHi());

		// uninstall during dev
		BundleContext context = launcher.framework.getBundleContext();
		Bundle provider = context.getBundle("file:../doug.embed.provider");
		provider.uninstall();

		launcher.stop();
	}

	public IDougService getDougService() {
		if (dougService == null) {
			BundleContext context = framework.getBundleContext();
			ServiceReference<IDougService> ref = context.getServiceReference(IDougService.class);
			dougService = context.getService(ref);
		}
		return dougService;
	}

}
