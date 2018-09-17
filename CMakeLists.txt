cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ct/cmake")
include(compilerSettings) # needs to be called before first project()

project(ct)
# set(CMAKE_DEBUG_TARGET_PROPERTIES INCLUDE_DIRECTORIES)


include(explicitTemplateHelpers)
include(clang-cxx-dev-tools)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0057 NEW)
include(External_GTest)
enable_testing()
find_package(GTest REQUIRED)

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread -std=c++11 -Wall")
SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(HEADER_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}/include/ct" CACHE PATH "Where to install robcogen-generated headers")
set(LIB_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}/lib/ct" CACHE PATH "Where to install robcogen-generated library")

if(NOT ENV{BLASFEO_DIR})
  set( ENV{BLASFEO_DIR} "${CMAKE_INSTALL_PREFIX}" )
endif()
if(NOT ENV{HPIPM_DIR})
  set( ENV{HPIPM_DIR} "${CMAKE_INSTALL_PREFIX}" )
endif()

add_subdirectory(ct_core)
add_subdirectory(ct_optcon)
add_subdirectory(ct_rbd)
add_subdirectory(ct_models)
