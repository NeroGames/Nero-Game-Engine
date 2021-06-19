# Cmake Version
#----------------------------------------------------------------------------------#
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
# Project Version
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# Environment Variable
#----------------------------------------------------------------------------------#
set(NERO_GAME_HOME $ENV{NERO_GAME_HOME})
#[[
    Nero Game Home Directory Structure

    Nero Game Engien r2.0.0 <--- [Home]
        Editor
        Compiler
        Library
        Nerolib
        Tools
#]]

# Cmake Module
#----------------------------------------------------------------------------------#
set(CMAKE_MODULE_PATH "${NERO_GAME_HOME}/Tools/Cmake/share/cmake-3.17/Modules")

# Output Directory
#----------------------------------------------------------------------------------#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler Configuration
#----------------------------------------------------------------------------------#
# Activate C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++1z)
# Remove Console
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
# Compile Option
add_definitions(-lstdc++fs)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DSOL_ALL_SAFETIES_ON=1)
add_definitions(-DSOL_USING_CXX_LUA)
add_definitions(-DBUILD_WITH_EASY_PROFILER)
add_definitions(-DNERO_GAME_DEBUG)
add_definitions(-DNERO_GAME_DEVMODE)

# Include Files & Libraries
#----------------------------------------------------------------------------------#
# Make Include Files Visible
include_directories ("include")
include_directories("library")
include_directories("${NERO_GAME_HOME}/Library/include")
# Make Libraries Visible
link_directories("${NERO_GAME_HOME}/Library/lib")
link_directories("${NERO_GAME_HOME}/Library/bin")
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
# Libgit2
set(LIBGIT2_LIBS "-lgit2.dll")
# Lua
set(LUA_LIBS "-llua")
# CPP_FILESYSTEM
set(CPP_FILESYSTEM "stdc++fs")
# EASY_PROFILER
set(EASY_PROFILER "-leasy_profiler.dll")
# POCO
set(POCO_DEP "-liphlpapi -lcrypto -lssl -lexpat -lzlibstatic -lws2_32")
set(POCO "-lPocoRedis -lPocoPDF -lPocoCppParser -lPocoZip -lPocoDataSQLite -lPocoData -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoJSON -lPocoXML -lPocoEncodings -lPocoFoundation ${POCO_DEP}")

# Nero Game Core Library
#----------------------------------------------------------------------------------#
# Engine Core Library Source Files
file(GLOB_RECURSE NERO_GAME_CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/core/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/ltbl/*")
# Dependencies
set(NERO_GAME_CORE_DEP ${SFML_LIBS} ${OPENGL_gl_LIBRARY} ${LUA_LIBS} ${CPP_FILESYSTEM}  ${Box2D_LIBS} ${EASY_PROFILER} ${POCO})
# Build Nero Game Core
add_library(nerogame-core ${NERO_GAME_CORE_SOURCES} )
target_link_libraries(nerogame-core ${NERO_GAME_CORE_DEP})

# Install Nero Game Core Library
#----------------------------------------------------------------------------------#
#[[#]]
add_custom_command(TARGET nerogame-core
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory    "${NERO_GAME_HOME}/Nerolib/include/Nero/core"
        COMMAND ${CMAKE_COMMAND} -E make_directory      "${NERO_GAME_HOME}/Nerolib/include/Nero/core"
        COMMAND ${CMAKE_COMMAND} -E make_directory      "${NERO_GAME_HOME}/Nerolib/lib/"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:nerogame-core>" "${NERO_GAME_HOME}/Nerolib/lib/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/include/Nero/core" "${NERO_GAME_HOME}/Nerolib/include/Nero/core")
#]]
# Nero Game Render Engine (Desktop)
#----------------------------------------------------------------------------------#
# Render Engine Source Files
file(GLOB_RECURSE NERO_GAME_RENDERER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/renderer/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
# Dependencies
set(NERO_GAME_RENDERER_DEP nerogame-core ${NERO_GAME_CORE_DEP})
# Build Nero Render Egnine
add_executable(RenderEngine ${NERO_GAME_RENDERER_SOURCES})
target_link_libraries(RenderEngine ${NERO_GAME_RENDERER_DEP})
set_target_properties(RenderEngine PROPERTIES OUTPUT_NAME "Render Engine")

# Nero Game Editor (Desktop)
#----------------------------------------------------------------------------------#
# Engine Editor Source Files
file(GLOB_RECURSE NERO_GAME_EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/editor/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/iconfont/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/nativefiledialog/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/nodeeditor/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
# Dependencies
set(NERO_GAME_EDITOR_DEP nerogame-core ${NERO_GAME_CORE_DEP} comctl32.lib ${LIBGIT2_LIBS})
# Build Nero Game Editor
add_executable(GameEditor ${NERO_GAME_EDITOR_SOURCES})
target_link_libraries(GameEditor ${NERO_GAME_EDITOR_DEP})
set_target_properties(GameEditor PROPERTIES OUTPUT_NAME "Nero Game Engine")

# Install Nero Game Editor
#----------------------------------------------------------------------------------#
#[[#]]
add_custom_command(TARGET nerogame-core
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/template"
        COMMAND ${CMAKE_COMMAND} -E copy_directory      "${CMAKE_SOURCE_DIR}/template" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/template")
#]]


# Nero Game Console
#----------------------------------------------------------------------------------#
# Engine Console Source Files
file(GLOB_RECURSE NERO_GAME_CONSOLE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Nero/console/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console/*")
# Dependencies
set(NERO_GAME_CORE_DEP ${SFML_LIBS} ${OPENGL_gl_LIBRARY} ${LUA_LIBS} ${CPP_FILESYSTEM}  ${Box2D_LIBS} ${EASY_PROFILER} ${POCO})
# Build Nero Game Core
add_executable(Console ${NERO_GAME_CONSOLE_SOURCES} )
set_target_properties(Console PROPERTIES OUTPUT_NAME "nero")

# Doxygen Documentation
#----------------------------------------------------------------------------------#
# check if Doxygen is installed
#[[
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # build the docs
    add_custom_target(Doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
#]]



# Test Laboratory
#----------------------------------------------------------------------------------#
# Test Lab Source Files
file(GLOB_RECURSE TEST_LABORATORY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/testlab/*")
# Dependencies
set(TEST_LABORATORY_DEP nerogame-core ${NERO_GAME_CORE_DEP})
# Build Test Laboratory
add_executable(TestLab ${TEST_LABORATORY_SOURCES})
target_link_libraries(TestLab ${TEST_LABORATORY_DEP})
