# This script locates eXpl0it3r's port of the Box2D library.
#
# USAGE
#
# By default, the dynamic version of Box2D will be found. To find the static
# one instead, you must set the BOX2D_STATIC_LIBRARIES variable to TRUE before
# calling find_package(BOX2D). In that case BOX2D_STATIC will also be defined
# by this script. Example:
#
# set(BOX2D_STATIC_LIBRARIES TRUE)
# find_package(BOX2D)
#
# If Box2D is not installed in a standard path, you can use the BOX2DDIR or
# BOX2D_ROOT CMake (or environment) variables to tell CMake where to look for
# Box2D.
#
#
# OUTPUT
#
# This script defines the following variables:
#   - BOX2D_LIBRARY_DEBUG:   the path to the debug library
#   - BOX2D_LIBRARY_RELEASE: the path to the release library
#   - BOX2D_LIBRARY:         the path to the library to link to
#   - BOX2D_FOUND:           true if the Box2D library is found
#   - BOX2D_INCLUDE_DIR:     the path where Box2D headers are located (the directory containing the Box2D/Box2D.hpp file)
#
#
# EXAMPLE
#
# find_package(Box2D REQUIRED)
# include_directories(${BOX2D_INCLUDE_DIR})
# add_executable(myapp ...)
# target_link_libraries(myapp ${BOX2D_LIBRARY} ...)

set(BOX2D_FOUND FALSE)

if(BOX2D_STATIC_LIBRARIES)
  set(BOX2D_SUFFIX "-s")
  add_definitions(-DBOX2D_STATIC)
else()
  set(BOX2D_SUFFIX "")
endif()

find_path(
  BOX2D_INCLUDE_DIR
  Box2D/Box2D.hpp
  PATH_SUFFIXES
    include
  PATHS
    /usr
    /usr/local
    ${BOX2DDIR}
    ${BOX2D_ROOT}
    $ENV{BOX2DDIR}
    $ENV{BOX2D_ROOT}
)

find_library(
  BOX2D_LIBRARY_RELEASE
  box2d${BOX2D_SUFFIX}
  PATH_SUFFIXES
    lib
    lib64
  PATHS
    /usr
    /usr/local
    ${BOX2DDIR}
    ${BOX2D_ROOT}
    $ENV{BOX2DDIR}
    $ENV{BOX2D_ROOT}
)

find_library(
  BOX2D_LIBRARY_DEBUG
  box2d${BOX2D_SUFFIX}-d
  PATH_SUFFIXES
    lib
    lib64
  PATHS
    /usr
    /usr/local
    ${BOX2DDIR}
    ${BOX2D_ROOT}
    $ENV{BOX2DDIR}
    $ENV{BOX2D_ROOT}
)

if(BOX2D_LIBRARY_RELEASE AND BOX2D_LIBRARY_DEBUG)
  set(BOX2D_LIBRARY debug ${BOX2D_LIBRARY_DEBUG} optimized ${BOX2D_LIBRARY_RELEASE})
endif()

if(BOX2D_LIBRARY_RELEASE AND NOT BOX2D_LIBRARY_DEBUG)
  set(BOX2D_LIBRARY_DEBUG ${BOX2D_LIBRARY_RELEASE})
  set(BOX2D_LIBRARY ${BOX2D_LIBRARY_RELEASE})
endif()

if(BOX2D_LIBRARY_DEBUG AND NOT BOX2D_LIBRARY_RELEASE)
  set(BOX2D_LIBRARY_RELEASE ${BOX2D_LIBRARY_DEBUG})
  set(BOX2D_LIBRARY ${BOX2D_LIBRARY_DEBUG})
endif()

if(NOT BOX2D_INCLUDE_DIR OR NOT BOX2D_LIBRARY)
  if(BOX2D_FIND_REQUIRED)
    message(FATAL_ERROR "Box2D not found.")
  elseif(NOT BOX2D_FIND_QUIETLY)
    message("Box2D not found.")
  endif()
else()
  set(BOX2D_FOUND true)
  if (NOT BOX2D_FIND_QUIETLY)
    message(STATUS "Box2D found: ${BOX2D_INCLUDE_DIR}")
  endif()
endif()
# This script locates eXpl0it3r's port of the Box2D library.
#
# USAGE
#
# By default, the dynamic version of Box2D will be found. To find the static
# one instead, you must set the BOX2D_STATIC_LIBRARIES variable to TRUE before
# calling find_package(BOX2D). In that case BOX2D_STATIC will also be defined
# by this script. Example:
#
# set(BOX2D_STATIC_LIBRARIES TRUE)
# find_package(BOX2D)
#
# If Box2D is not installed in a standard path, you can use the BOX2DDIR or
# BOX2D_ROOT CMake (or environment) variables to tell CMake where to look for
# Box2D.
#
#
# OUTPUT
#
# This script defines the following variables:
#   - BOX2D_LIBRARY_DEBUG:   the path to the debug library
#   - BOX2D_LIBRARY_RELEASE: the path to the release library
#   - BOX2D_LIBRARY:         the path to the library to link to
#   - BOX2D_FOUND:           true if the Box2D library is found
#   - BOX2D_INCLUDE_DIR:     the path where Box2D headers are located (the directory containing the Box2D/Box2D.hpp file)
#
#
# EXAMPLE
#
# find_package(Box2D REQUIRED)
# include_directories(${BOX2D_INCLUDE_DIR})
# add_executable(myapp ...)
# target_link_libraries(myapp ${BOX2D_LIBRARY} ...)

set(BOX2D_FOUND FALSE)

if(BOX2D_STATIC_LIBRARIES)
  set(BOX2D_SUFFIX "-s")
  add_definitions(-DBOX2D_STATIC)
else()
  set(BOX2D_SUFFIX "")
endif()

find_path(
  BOX2D_INCLUDE_DIR
  Box2D/Box2D.hpp
  PATH_SUFFIXES
    include
  PATHS
    /usr
    /usr/local
    ${BOX2DDIR}
    ${BOX2D_ROOT}
    $ENV{BOX2DDIR}
    $ENV{BOX2D_ROOT}
)

find_library(
  BOX2D_LIBRARY_RELEASE
  box2d${BOX2D_SUFFIX}
  PATH_SUFFIXES
    lib
    lib64
  PATHS
    /usr
    /usr/local
    ${BOX2DDIR}
    ${BOX2D_ROOT}
    $ENV{BOX2DDIR}
    $ENV{BOX2D_ROOT}
)

find_library(
  BOX2D_LIBRARY_DEBUG
  box2d${BOX2D_SUFFIX}-d
  PATH_SUFFIXES
    lib
    lib64
  PATHS
    /usr
    /usr/local
    ${BOX2DDIR}
    ${BOX2D_ROOT}
    $ENV{BOX2DDIR}
    $ENV{BOX2D_ROOT}
)

if(BOX2D_LIBRARY_RELEASE AND BOX2D_LIBRARY_DEBUG)
  set(BOX2D_LIBRARY debug ${BOX2D_LIBRARY_DEBUG} optimized ${BOX2D_LIBRARY_RELEASE})
endif()

if(BOX2D_LIBRARY_RELEASE AND NOT BOX2D_LIBRARY_DEBUG)
  set(BOX2D_LIBRARY_DEBUG ${BOX2D_LIBRARY_RELEASE})
  set(BOX2D_LIBRARY ${BOX2D_LIBRARY_RELEASE})
endif()

if(BOX2D_LIBRARY_DEBUG AND NOT BOX2D_LIBRARY_RELEASE)
  set(BOX2D_LIBRARY_RELEASE ${BOX2D_LIBRARY_DEBUG})
  set(BOX2D_LIBRARY ${BOX2D_LIBRARY_DEBUG})
endif()

if(NOT BOX2D_INCLUDE_DIR OR NOT BOX2D_LIBRARY)
  if(BOX2D_FIND_REQUIRED)
    message(FATAL_ERROR "Box2D not found.")
  elseif(NOT BOX2D_FIND_QUIETLY)
    message("Box2D not found.")
  endif()
else()
  set(BOX2D_FOUND true)
  if (NOT BOX2D_FIND_QUIETLY)
    message(STATUS "Box2D found: ${BOX2D_INCLUDE_DIR}")
  endif()
endif()