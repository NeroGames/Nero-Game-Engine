# Cmake version
cmake_minimum_required(VERSION 3.15)

# Utility
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
# Project Name
project("Nero Game Engine 2")
# Project version
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# Compiler Configuration
#----------------------------------------------------------------------------------#
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
add_definitions(-DNERO_GAME_DEBUG)
add_definitions(-DBUILD_WITH_EASY_PROFILER)

# Include Files & Libraries
#----------------------------------------------------------------------------------#
# Make include file visible
include_directories ("include")
include_directories("library")
include_directories("../../Library/include")
# Make libraries visible
link_directories("../../Library/lib" "../../Library/bin")
# Find OpenGL
find_package(OpenGL)
# SFML
set(SFML_LIBS "-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network")
# Box2d
set(Box2d_LIBS "-lBox2D.dll")
# Zilb
set(ZLIB_LIBS "-lzlib.dll")
# OpenSSL
set(OPENSSL_LIBS "-lcrypto.dll -lssl.dll")
# Libssh2
set(LIBSSH2_LIBS "-llibssh2.dll")
# Libgit2
set(HTTP_PARSER "-lhttp_parser")
# Libgit2
set(LIBGIT2_LIBS "-lgit2.dll")
# Lua
set(LUA_LIBS "-llua")
# CPP_FILESYSTEM
set(CPP_FILESYSTEM "stdc++fs")
# EASY_PROFILER
set(EASY_PROFILER "-leasy_profiler.dll")
# POCO
set(POCO "-lPocoRedis -lPocoPDF -lPocoCppParser -lPocoZip -lPocoData -lPocoNetSSL -lPocoCrypto  -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoEncodings -lPocoFoundation -lexpat -liphlpapi -lssl -lcrypto -lzlibstatic -lws2_32")

# Nero Game Core Library
#----------------------------------------------------------------------------------#
# Engine Core Library Source Files
file(GLOB_RECURSE NERO_GAME_CORE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/core/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/easyloggingpp/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/ltbl/*")
# Dependencies
set(NERO_GAME_CORE_DEP ${SFML_LIBS} ${OPENGL_gl_LIBRARY} ${LUA_LIBS} ${CPP_FILESYSTEM}  ${Box2d_LIBS} ${EASY_PROFILER} ${POCO})
# Build Nero Game Core
add_library(nerogame-core ${NERO_GAME_CORE_SRC} )
target_link_libraries(nerogame-core ${NERO_GAME_CORE_DEP})

# Nero Game Renderer (Desktop)
#----------------------------------------------------------------------------------#
# Engine Renderer Source Files
file(GLOB_RECURSE NERO_GAME_RENDERER_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/renderer/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
# Dependencies
set(NERO_GAME_RENDERER_DEP nerogame-core ${NERO_GAME_CORE_DEP})
# Build Nero Game Renderer
add_executable(EngineRenderer ${NERO_GAME_RENDERER_SRC})
target_compile_options(EngineRenderer PRIVATE "-mwindows")
target_link_libraries(EngineRenderer ${NERO_GAME_RENDERER_DEP})

# Nero Game Editor (Desktop)
#----------------------------------------------------------------------------------#
# Engine Editor Source Files
file(GLOB_RECURSE NERO_GAME_EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/editor/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/IconFontCppHeaders/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/nativefiledialog/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/nodeeditor/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
# Dependencies
set(NERO_GAME_EDITOR_DEP nerogame-core ${NERO_GAME_CORE_DEP} comctl32.lib ${ZLIB_LIBS} ${OPENSSL_LIBS} ${LIBSSH2_LIBS} ${HTTP_PARSER} ${LIBGIT2_LIBS} ${LUA_LIBS})
# Build Nero Game Editor
add_executable(EngineEditor ${NERO_GAME_EDITOR_SOURCES})
target_link_libraries(EngineEditor ${NERO_GAME_EDITOR_DEP})


# Test Laboratory
#----------------------------------------------------------------------------------#
# Engine Renderer Source Files
file(GLOB_RECURSE TEST_LABORATORY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/testlab/*")
# Dependencies
set(TEST_LABORATORY_DEP nerogame-core ${NERO_GAME_CORE_DEP})
# Build Test Laboratory
add_executable(TestLab ${TEST_LABORATORY_SRC})
target_link_libraries(TestLab ${TEST_LABORATORY_DEP})

# Doxygen Documentation
#----------------------------------------------------------------------------------#
