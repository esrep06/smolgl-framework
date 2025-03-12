file (GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Game_Of_Life ${SOURCES})
set_property(TARGET Game_Of_Life PROPERTY CXX_STANDARD 20)
# set_target_properties(Game_Of_Life PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../build/src/game_of_life/")

target_include_directories(Game_Of_Life PRIVATE ${EXTERNAL_INCLUDES})

target_link_libraries(Game_Of_Life Engine)

