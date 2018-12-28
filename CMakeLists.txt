# Cmake version
cmake_minimum_required (VERSION 2.8)

# Enable c++11
set (CMAKE_CXX_STANDARD 11)

# Specify default build type if none provided
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Specify default cmake module path if none provided
if(NOT CMAKE_MODULE_PATH)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules CACHE STRING "The path to the cmake modules. This path must contain FindXXX.cmake files." FORCE)
endif()

# The project 
project (Nero)

#set(NERO_MAJOR_VERSION 0)
#set(NERO_MINOR_VERSION 9)
#set(NERO_REVISION_VERSION 0)

#set(NERO_BUILD_TYPE	Release CACHE STRING "Choose the build type (Debug / Release)")
set(NERO_BUILD_SHARED_LIB ON CACHE BOOL "Build shared or static library")
#set(NERO_BUILD_DOC 	FALSE CACHE BOOL "Build doxygen doc")
#set(NERO_BUILD_EXEMPLES FALSE CACHE BOOL "Build all exemples")

add_definitions(-DAUTO_INITIALIZE_EASYLOGGINGPP)


# Get the engine source files
include_directories (include)
include_directories(extlibs)
file(GLOB_RECURSE NERO_SOURCES *.h *.cpp *.hpp *.cc)


find_package(SFML 2.4 REQUIRED COMPONENTS graphics window  network audio system)
find_package(SFGUI REQUIRED)
find_package(BOX2D REQUIRED)
find_package(LUA REQUIRED)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.61.0 REQUIRED COMPONENTS filesystem system) 

include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS}) 

# Build the library
if(${NERO_BUILD_SHARED_LIB})
   add_library(nero SHARED ${NERO_SOURCES})
else()
	add_library(nero  ${NERO_SOURCES})
endif()

# Link Depedencies
target_link_libraries(nero ${SFML_LIBRARIES} ${SFGUI_LIBRARY} ${BOX2D_LIBRARY} ${LUA_LIBRARIES} ${Boost_LIBRARIES})

# Setup the output library
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)