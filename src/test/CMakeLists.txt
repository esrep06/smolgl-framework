file (GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Test ${SOURCES})
set_property(TARGET Test PROPERTY CXX_STANDARD 20)

target_include_directories(Test PRIVATE ${EXTERNAL_INCLUDES})

target_link_libraries(Test Engine)

