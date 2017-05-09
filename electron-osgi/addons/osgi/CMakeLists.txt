include(NodeModule)

find_package(JNI)

add_node_module(osgi osgi.cpp)
target_include_directories(osgi PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(osgi ${JNI_LIBRARIES})
