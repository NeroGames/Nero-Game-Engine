# DO NOT MODIFY
#----------------------------------------------------------------------------------#
#[[
    This document as been generated automatically by the Nero Game Engine EditorZ.
    Any modification to this document can break the Build of your Game.
    Do not modify this document if you are not familiar with CMAKE.

    Learn more at https://nero-game.com/v2/game-project-compilation
#]]

# Cmake Version
#----------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.15)

# The Project
#----------------------------------------------------------------------------------#
# Project Name
project("Nero Game - ::Project_Name::")
# Project Version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# Environment Variable
#----------------------------------------------------------------------------------#
set(NERO_GAME_HOME $ENV{NERO_GAME_HOME})

# Cmake Module
#----------------------------------------------------------------------------------#
set(CMAKE_MODULE_PATH "${NERO_GAME_HOME}/Tools/Cmake/share/cmake-3.17/Modules")

# Compiler Configuration
#----------------------------------------------------------------------------------#
# Activate C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++1z)
# Remove console
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
# Compile Option
add_definitions(-lstdc++fs)
add_definitions(-DBOOST_DLL_FORCE_ALIAS_INSTANTIATION)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DSOL_ALL_SAFETIES_ON=1)
add_definitions(-DSOL_USING_CXX_LUA)

# Include Files & Libraries
#----------------------------------------------------------------------------------#
# Make Include Files Visible
include_directories ("${NERO_GAME_HOME}/Library/include")
# Make Libraries Visible
link_directories("${NERO_GAME_HOME}/Library/lib" "${NERO_GAME_HOME}/Library/bin")
# Find OpenGL
find_package(OpenGL)
# SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
# Box2D
set(Box2D_LIBS "-lBox2D.dll")
# OpenSSL
set(OPENSSL_LIBS "-lcrypto.dll -lssl.dll")
# Libssh2
set(LIBSSH2_LIBS "-llibssh2.dll")
# Lua
set(LUA_LIBS "-llua")
# CPP_FILESYSTEM
set(CPP_FILESYSTEM "stdc++fs")
# EASY_PROFILER
set(EASY_PROFILER "-leasy_profiler.dll")
# POCO
set(POCO_DEP "-liphlpapi -lcrypto -lssl -lexpat -lzlibstatic -lws2_32")
set(POCO "-lPocoRedis -lPocoPDF -lPocoCppParser -lPocoZip -lPocoDataSQLite -lPocoData -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoJSON -lPocoXML -lPocoEncodings -lPocoFoundation ${POCO_DEP}")
set(NERO_GAME_CORE "-lnerogame-core")

# Project Library
#----------------------------------------------------------------------------------#
# Project Sources
file(GLOB_RECURSE PROJECT_SOURCES *.h *.cpp)
# Dependencies
set(NERO_GAME_CORE_DEP ${SFML_LIBS} ${OPENGL_gl_LIBRARY} ${LUA_LIBS} ${CPP_FILESYSTEM}  ${Box2D_LIBS} ${EASY_PROFILER} ${POCO})
set(PROJECT_DEP ${NERO_GAME_CORE} ${NERO_GAME_CORE_DEP})
# Build Game Library
add_library(::Project_Library:: SHARED ${PROJECT_SOURCES})
target_link_libraries(::Project_Library:: ${PROJECT_DEP})
