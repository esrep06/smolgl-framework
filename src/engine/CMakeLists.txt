set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "${EXTERNAL_INCLUDES}/include/engine/*.hpp")

if (UNIX AND NOT APPLE)
    set(LIBS "${EXTERNAL_LIBRARIES}/linux/libGLEW.a" "${EXTERNAL_LIBRARIES}/linux/libglfw3.a")
elseif (APPLE)
    set(LIBS "${EXTERNAL_LIBRARIES}/darwin/libGLEW.a" "${EXTERNAL_LIBRARIES}/darwin/libglfw3.a")
elseif (WIN32)
    set(LIBS "${EXTERNAL_LIBRARIES}/win/glew32s.lib" "${EXTERNAL_LIBRARIES}/win/glfw3.lib" "${EXTERNAL_LIBRARIES}/win/OpenGL32.lib")
endif()

add_library(Engine ${SOURCES} ${HEADERS})

if (UNIX AND NOT APPLE)
    target_link_libraries(Engine PRIVATE ${LIBS} GL)
elseif (APPLE)
    target_link_libraries(Engine PRIVATE ${LIBS} 
                            "-framework OpenGL" 
                            "-framework CoreVideo"
                            "-framework Cocoa"
                            "-framework IOKit"
                            "-framework QuartzCore")
elseif (WIN32)
    target_link_libraries(Engine PRIVATE ${LIBS})
endif()

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

target_include_directories(Engine PRIVATE "${EXTERNAL_INCLUDES}") 

add_compile_definitions(GLEW_STATIC)

