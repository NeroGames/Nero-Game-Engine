# Setup Cmake version
cmake_minimum_required(VERSION 3.15)

# Define Project
project("[Nero Game] ::Project_Name::")

# Setup Engine Directory
#set(NERO_ENGINE_INSTALLATION_DIR "::Engine_Installation_Directory::")

###################################################################################################################
# Todo : Remove - For developement only
set(NERO_ENGINE_INSTALLATION_DIR "C:/Users/sk-landry/Desktop/Origin/Project/Nero Game Engine")
##################################################################################################################

# enable C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++1z)

# Add Compiler definitions
add_definitions(-DBOOST_DLL_FORCE_ALIAS_INSTANTIATION)
add_definitions(-lstdc++fs)

# Setup Compiler
#SET(CMAKE_C_COMPILER   "${NERO_ENGINE_INSTALLATION_DIR}/compiler/mingw730_32/bin/gcc")
#SET(CMAKE_CXX_COMPILER "${NERO_ENGINE_INSTALLATION_DIR}/compiler/mingw730_32/bin/g++")

# Setup Engine Library
include_directories("${NERO_ENGINE_INSTALLATION_DIR}/Library/include")
link_directories("${NERO_ENGINE_INSTALLATION_DIR}/Library/lib" "${NERO_ENGINE_INSTALLATION_DIR}/Library/bin")

###################################################################################################################
# Todo : Remove - For developement only
# Link the project to the nero lib
include_directories("C:/Users/sk-landry/Desktop/Origin/Project/Nero Game Engine/Project/Nero Engine 2/include")
link_directories("C:/Users/sk-landry/Desktop/Origin/Project/Nero Game Engine/Project/build-Nero-Engine-2-Build-Default")
##################################################################################################################


# Find Packages
find_package(OpenGL)

#SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
#Box2d
set(Box2d_LIBS "-lBox2D.dll")
#Lua
set(LUA_LIBS "-llua")
#CPP_FILESYSTEM
set(CPP_FILESYSTEM "stdc++fs")
# NERO ENGINE
#set(NERO_ENGINE_CORE "-lnerogame-core.dll")

###################################################################################################################
# Todo : Remove - For developement only
set(NERO_ENGINE_CORE "C:/Users/sk-landry/Desktop/Origin/Project/Nero Game Engine/Project/build-Nero-Engine-2-Build-Default/libnerogame-core.a")
##################################################################################################################

# Project Source
file(GLOB_RECURSE PROJECT_SOURCES *.h *.cpp)

# Generate Game Library
add_library(::Project_Library:: SHARED ${PROJECT_SOURCES})

# Link other libraries to Game Library
target_link_libraries(::Project_Library:: ${NERO_ENGINE_CORE} ${SFML_LIBS} ${OPENGL_gl_LIBRARY} ${LUA_LIBS} ${CPP_FILESYSTEM}  ${Box2d_LIBS})

