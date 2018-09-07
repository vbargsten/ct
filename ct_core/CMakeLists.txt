project (ct_core)

# find_package(catkin REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS REQUIRED)

include_directories(
  ${BOOST_INCLUDE_DIRS}
)

include_directories (
    include
    include/ct/external
    ${EIGEN3_INCLUDE_DIR}
)

## dummy prespec libs
set(PRESPEC_LIB_NAMES "")

## assemble list of libraries that contain prespecified templates
if(USE_PRESPEC)
    ct_configure_explicit_templates("${CMAKE_CURRENT_SOURCE_DIR}/../ct/config/explicit_templates.cfg" "${CMAKE_CURRENT_SOURCE_DIR}/prespec/" "ct_core")
    message(WARNING "CT Core: Compiling the following explict template libraries: ${PRESPEC_LIB_NAMES}")
endif(USE_PRESPEC)

#used later for clang-tidy
set(INC_DIRS ${BOOST_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/external" ${EIGEN3_INCLUDE_DIR})

# catkin_package(
#    INCLUDE_DIRS
#         ${BOOST_INCLUDE_DIRS}
#         include
#         include/external
#         examples
#         ${EIGEN3_INCLUDE_DIR}
#    LIBRARIES
#        dl
#        ct_core
#        ${PRESPEC_LIB_NAMES}
#    DEPENDS
#        EIGEN3
# )


set(CODEGEN_TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/templates")
set(CODEGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/ct/core/templateDir.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ct/core/templateDir.h)

add_library(ct_core SHARED
   src/core/internal/autodiff/SparsityPattern.cpp
)
target_include_directories(ct_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/external" ${EIGEN3_INCLUDE_DIR})
# add libraries for explicit templates
target_link_libraries(ct_core 
    ${CMAKE_DL_LIBS}
)
# if(USE_PRESPEC)
#     ct_add_explicit_template_libs()
# endif(USE_PRESPEC)

install(TARGETS ct_core DESTINATION lib/ct)
install (
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION include
    FILES_MATCHING 
    PATTERN "*.hpp"
    PATTERN "*.h"
)

## build doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/ct_core.doxyfile)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
        COMMAND ${CMAKE_COMMAND} -E echo_append "API Documentation built in ${CMAKE_CURRENT_SOURCE_DIR}/doc"
        VERBATIM)
endif()

# ct_configure_clang_tidy(INC_DIRS)
