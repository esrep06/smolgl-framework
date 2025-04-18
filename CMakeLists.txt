cmake_minimum_required (VERSION 3.8)

project (SmolGL_Engine)

set(EXTERNAL_INCLUDES "${PROJECT_SOURCE_DIR}/include")
set(EXTERNAL_LIBRARIES "${PROJECT_SOURCE_DIR}/lib")

add_subdirectory("src/engine")
add_subdirectory(src/virus_io)
add_subdirectory(src/test)
add_subdirectory(src/battleship)

