include(ExternalProject)

ExternalProject_Add(iojs
    PREFIX ${CMAKE_BINARY_DIR}/iojs
    URL https://atom.io/download/electron/v1.6.6/iojs-v1.6.6.tar.gz
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(iojs source_dir)
set(iojsIncludeDir ${source_dir})
set(iojsIncludes ${iojsIncludeDir}/src ${iojsIncludeDir}/deps/v8/include ${iojsIncludeDir}/deps/uv/include)

macro(add_node_module target)
    add_library(${target} SHARED ${ARGN})
    set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX ".node")
    target_include_directories(${target} PRIVATE ${iojsIncludes})
    target_link_libraries(${target} "-undefined dynamic_lookup")
    add_dependencies(${target} iojs)
    install(TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/lib)
endmacro(add_node_module)
