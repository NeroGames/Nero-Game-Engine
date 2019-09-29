# Cmake version
cmake_minimum_required(VERSION 3.0)

# Configuration
#----------------------------------------------------------------------------------#
# Define a macro that helps defining an option
#macro(nero_set_option var default type docstring)
#    if(NOT DEFINED ${var})
#        set(${var} ${default})
#    endif()
#    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
#endmacro()

# The Project
#----------------------------------------------------------------------------------#
project("Nero-Game-Engine-2")

# Project version
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
#----------------------------------------------------------------------------------#

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++1z)
set(DCMAKE_C_FLAGS "-mwindows")

SET(GUI_TYPE WIN32)


IF(WIN32)
SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)


add_definitions(-lstdc++fs)

add_definitions(-DAUTO_INITIALIZE_EASYLOGGINGPP)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_DLL_USE_STD_FS)

# Get the engine source files
include_directories ("include")
include_directories("library")
include_directories("project")
include_directories("example")
file(GLOB_RECURSE NERO_GAME_ENGINE_SOURCES *.h *.cpp *.hpp *.cc *.md *.c)

file(GLOB_RECURSE NERO_GAME_LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/core/*" "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*" "${CMAKE_CURRENT_SOURCE_DIR}/library/*")

# setup Library
include_directories("../../Library/include")
link_directories("../../Library/lib" "../../Library/bin")

find_package(OpenGL)

#SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
#Box2d
set(Box2d_LIBS "libBox2D.dll")
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

add_executable(${PROJECT_NAME} "main.cpp" ${NERO_GAME_ENGINE_SOURCES})
add_library(nero-game-core SHARED ${NERO_GAME_LIBRARY_SOURCES})


target_link_libraries(${PROJECT_NAME} comctl32.lib stdc++fs ${SFML_LIBS} ${Box2d_LIBS} ${OPENGL_gl_LIBRARY} ${ZLIB_LIBS} ${OPENSSL_LIBS} ${LIBSSH2_LIBS} ${HTTP_PARSER} ${LIBGIT2_LIBS})

target_link_libraries(nero-game-core comctl32.lib stdc++fs ${SFML_LIBS} ${Box2d_LIBS} ${OPENGL_gl_LIBRARY} ${ZLIB_LIBS} ${OPENSSL_LIBS} ${LIBSSH2_LIBS} ${HTTP_PARSER} ${LIBGIT2_LIBS})
