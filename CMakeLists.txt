cmake_minimum_required(VERSION 2.8)

#Project Nero Game Engine 2.0
project(Nero_Game_Engine_2)

# Get the engine source files
include_directories ("include")
include_directories("library")
file(GLOB_RECURSE NERO_GAME_ENGINE_SOURCES *.h *.cpp *.hpp *.cc)

# setup Library
include_directories("../../Library/include")
link_directories("../../Library/lib" "../../Library/bin")

find_package(OpenGL)

#SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
#Box2d
set(Box2d_LIBS "")

add_executable(${PROJECT_NAME} "main.cpp" ${NERO_GAME_ENGINE_SOURCES})

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} ${Box2d_LIBS} ${OPENGL_gl_LIBRARY})
