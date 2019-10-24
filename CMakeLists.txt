# Cmake version
cmake_minimum_required(VERSION 3.15)

# Configuration
#----------------------------------------------------------------------------------#
# Define a macro that helps defining an option
macro(nero_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# The Project
#----------------------------------------------------------------------------------#
project("Nero Game Engine 2")

# Project version
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# Activate C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++1z)
# Compile Option
add_definitions(-lstdc++fs)
add_definitions(-DAUTO_INITIALIZE_EASYLOGGINGPP)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DSOL_ALL_SAFETIES_ON=1)
add_definitions(-DSOL_USING_CXX_LUA)


# Get the engine source files
include_directories ("include")
include_directories("library")
#include_directories("project")
#include_directories("example")

# Engine Editor Source
file(GLOB_RECURSE NERO_GAME_EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/project/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/nativefiledialog/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/nodeeditor*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/easyloggingpp/*")


# Engine Core Source
file(GLOB_RECURSE NERO_GAME_CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/core/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/easyloggingpp/*")

# setup Library
include_directories("../../Library/include")
link_directories("../../Library/lib" "../../Library/bin")

find_package(OpenGL)

#SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
#Box2d
set(Box2d_LIBS "-lBox2D.dll")
#Zilb
set(ZLIB_LIBS "-lzlib.dll")
#OpenSSL
set(OPENSSL_LIBS "-lcrypto.dll -lssl.dll")
#Libssh2
set(LIBSSH2_LIBS "-llibssh2.dll")
#Libgit2
set(HTTP_PARSER "-lhttp_parser")
#Libgit2
set(LIBGIT2_LIBS "-lgit2.dll")
#Lua
set(LUA_LIBS "-llua")
#CPP_FILESYSTEM
set(CPP_FILESYSTEM "stdc++fs")


# Build Nero Game Core
add_library(nerogame-core ${NERO_GAME_CORE_SOURCES} )
target_link_libraries(nerogame-core ${SFML_LIBS} ${OPENGL_gl_LIBRARY} ${LUA_LIBS} ${CPP_FILESYSTEM}  ${Box2d_LIBS})

# Build Nero Game Editor
add_executable(Editor "main.cpp" ${NERO_GAME_EDITOR_SOURCES})
target_link_libraries(Editor comctl32.lib ${CPP_FILESYSTEM} ${SFML_LIBS} ${Box2d_LIBS} ${OPENGL_gl_LIBRARY} ${ZLIB_LIBS} ${OPENSSL_LIBS} ${LIBSSH2_LIBS} ${HTTP_PARSER} ${LIBGIT2_LIBS} ${LUA_LIBS})

