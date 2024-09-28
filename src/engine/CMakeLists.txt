set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "${EXTERNAL_INCLUDES}/include/engine/*.hpp")

if (UNIX OR APPLE)
    set(LIBS "${EXTERNAL_INCLUDES}/glew/lib/libGLEW.a" "${EXTERNAL_INCLUDES}/glfw/build/src/libglfw3.a")
endif()

add_library(Engine ${SOURCES} ${HEADERS})

if (UNIX)
    target_link_libraries(Engine PRIVATE ${LIBS} GL)
elseif (APPLE)
    target_link_libraries(Engine PRIVATE ${LIBS} 
                            "-framework OpenGL" 
                            "-framework CoreVideo"
                            "-framework Cocoa"
                            "-framework IOKit"
                            "-framework QuartzCore")
endif()

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

target_include_directories(Engine PRIVATE "${EXTERNAL_INCLUDES}") 

add_compile_definitions(GLEW_STATIC)

