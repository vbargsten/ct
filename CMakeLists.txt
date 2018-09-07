cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ct)

include(${CMAKE_CURRENT_SOURCE_DIR}/ct/cmake/compilerSettings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/ct/cmake/explicitTemplateHelpers.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/ct/cmake/clang-cxx-dev-tools.cmake)

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread -std=c++11 -Wall")
SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(ct_core)
add_subdirectory(ct_optcon)
add_subdirectory(ct_rbd)
add_subdirectory(ct_models)

