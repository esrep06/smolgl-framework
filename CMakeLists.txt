cmake_minimum_required (VERSION 3.8)

project (SmolGL_Engine)

set(EXTERNAL_INCLUDES "${PROJECT_SOURCE_DIR}/include")

add_subdirectory("src/engine")
add_subdirectory(src/test)

