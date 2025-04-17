cmake_minimum_required (VERSION 3.8)
file (GLOB_RECURSE SOURCES "./*.cpp")

add_executable(VirusIO ${SOURCES})
set_property(TARGET VirusIO PROPERTY CXX_STANDARD 20)

target_include_directories(VirusIO PRIVATE ${EXTERNAL_INCLUDES}  ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(VirusIO Engine)

