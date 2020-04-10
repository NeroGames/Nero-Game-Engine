# Setup Cmake version
cmake_minimum_required(VERSION 3.15)

# Define Project
project("::Project_Name::")

# Setup Engine Directory
set(NERO_ENGINE_INSTALLATION_DIR "::Engine_Installation_Directory::")

# Add Compiler definitions
add_definitions(-DBOOST_DLL_FORCE_ALIAS_INSTANTIATION)
add_definitions(-lstdc++fs)

# Setup Compiler
SET(CMAKE_C_COMPILER   "${NERO_ENGINE_INSTALLATION_DIR}/compiler/mingw730_32/bin/gcc")
SET(CMAKE_CXX_COMPILER "${NERO_ENGINE_INSTALLATION_DIR}/compiler/mingw730_32/bin/g++")

# Setup Engine Library
include_directories("${NERO_ENGINE_INSTALLATION_DIR}/Library/include")
link_directories("${NERO_ENGINE_INSTALLATION_DIR}/Library/lib" 
			     "${NERO_ENGINE_INSTALLATION_DIR}/Library/bin")

# Todo : remove - For developer purpose only
include_directories("C:/Users/sk-landry/Desktop/Origin/Project/Nero Game Engine/Project/Nero Engine 2/include")


# Find Packages
find_package(OpenGL)

# Retrieve libraries
# SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
# BOX2D
set(BOX2D_LIBS "libBox2D.dll")
# CPP FILESYSTEM
set(CPP_FILESYSTEM "stdc++fs")
# NERO ENGINE
set(NERO_ENGINE_CORE "libnerogame-core.dll")


# Project Source
file(GLOB_RECURSE PROJECT_SOURCES *.h *.cpp)

# Generate Game Library
add_library(::Project_Library:: SHARED ${PROJECT_SOURCES})

# Link other libraries to Game Library
target_link_libraries(::Project_Library:: ${SFML_LIBS} ${BOX2D_LIBS} ${CPP_FILESYSTEM} ${NERO_ENGINE_CORE})

