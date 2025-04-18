cmake_minimum_required (VERSION 3.8)
file (GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Battleship ${SOURCES})
set_property(TARGET Battleship PROPERTY CXX_STANDARD 20)

target_include_directories(Battleship PRIVATE ${EXTERNAL_INCLUDES}  ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(Battleship Engine)

