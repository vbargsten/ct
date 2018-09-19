project (ct_rbd)

# find_package(catkin REQUIRED ct_core ct_optcon)
find_package(kindr)

## assemble list of libraries that contain prespecified templates
if(USE_PRESPEC)
ct_configure_explicit_templates("${CMAKE_CURRENT_SOURCE_DIR}/../ct/config/explicit_templates.cfg" "${CMAKE_CURRENT_SOURCE_DIR}/prespec/" "ct_rbd")
message(WARNING "CT RBD: Compiling the following explict template libraries: ${PRESPEC_LIB_NAMES}")
endif(USE_PRESPEC)

#used later for clang-tidy
# set(INC_DIRS ${kindr_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")


# catkin_package(
#   INCLUDE_DIRS
#     ${kindr_INCLUDE_DIRS}
#     include
#     include/ct/rbd
#     include/ct/rbd/robot/kinematics/ikfast
#     test
#   LIBRARIES
#     ${PRESPEC_LIB_NAMES}
#   CATKIN_DEPENDS
#     ct_core
#     ct_optcon
#     kindr
# )

# include_directories(
#   ${kindr_INCLUDE_DIRS}
#   include/ct/rbd
#   include/ct/rbd/robot/kinematics/ikfast
#   test/models/testhyq/generated
#   include
#   test
# )


add_library(ct_rbd INTERFACE)
target_link_libraries(ct_rbd INTERFACE ct_optcon ct_core ${kindr_LIBS})
target_include_directories(ct_rbd INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/rbd/robot/kinematics/ikfast" ${kindr_INCLUDE_DIRS})

# this is for the broken includes, accessing subdir of rbd directly
target_include_directories(ct_rbd INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/rbd/")

#${kindr_INCLUDE_DIRS}
# add libraries for explicit template
if(USE_PRESPEC)
    ct_add_explicit_template_libs()
endif(USE_PRESPEC)


# hpipm and kindr currently can put the install include dir into our include_directories
# so make sure here we use the includes of the source tree in case ct has been 
# installed before by putting the source dir includes at the front
include_directories(BEFORE PRIVATE $<TARGET_PROPERTY:ct_rbd,INTERFACE_INCLUDE_DIRECTORIES>)
include_directories(BEFORE PRIVATE $<TARGET_PROPERTY:ct_core,INTERFACE_INCLUDE_DIRECTORIES>)
include_directories(BEFORE PRIVATE $<TARGET_PROPERTY:ct_optcon,INTERFACE_INCLUDE_DIRECTORIES>)

## TESTING

add_executable(RigidBodyPoseTest test/state/RigidBodyPoseTest.cpp)
target_link_libraries(RigidBodyPoseTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(RigidBodyPoseTest "" AUTO)

add_executable(JointStateTest test/state/JointStateTest.cpp)
target_link_libraries(JointStateTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(JointStateTest "" AUTO)

add_executable(RobotStateTest test/state/RobotStateTest.cpp)
target_link_libraries(RobotStateTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(RobotStateTest "" AUTO)

add_executable(RobCoGenContainerTest test/robot/robcogen/RobCoGenContainerTest.cpp)
target_link_libraries(RobCoGenContainerTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(RobCoGenContainerTest "" AUTO)

add_executable(KinematicsTest test/robot/kinematics/KinematicsTest.cpp)
target_link_libraries(KinematicsTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(KinematicsTest "" AUTO)

add_executable(KinematicsTestAd test/robot/kinematics/KinematicsTestAd.cpp)
target_link_libraries(KinematicsTestAd ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(KinematicsTestAd "" AUTO)

add_executable(OperationalSpaceTest test/operationalSpace/OperationalSpaceTest.cpp)
target_link_libraries(OperationalSpaceTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(OperationalSpaceTest "" AUTO)

add_executable(DataMapTests test/robot/dynamics/DataMapTests.cpp)
target_link_libraries(DataMapTests ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(DataMapTests "" AUTO)

add_executable(DynamicsTest test/robot/dynamics/DynamicsTests.cpp)
target_link_libraries(DynamicsTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(DynamicsTest "" AUTO)

add_executable(DynamicsTestFixBase test/robot/dynamics/DynamicsTestsFixBase.cpp)
target_link_libraries(DynamicsTestFixBase ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(DynamicsTestFixBase "" AUTO)

add_executable(FloatingBaseFDSystemTest test/systems/FloatingBaseFDSystemTest.cpp)
target_link_libraries(FloatingBaseFDSystemTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(FloatingBaseFDSystemTest "" AUTO)

add_executable(FixBaseFDSystemTest test/systems/FixBaseFDSystemTest.cpp)
target_link_libraries(FixBaseFDSystemTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(FixBaseFDSystemTest "" AUTO)

add_executable(ProjectedFDSystemTest test/systems/ProjectedFDSystemTest.cpp)
target_link_libraries(ProjectedFDSystemTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(ProjectedFDSystemTest "" AUTO)

add_executable(RBDLinearizerTest test/systems/linear/RBDLinearizerTest.cpp)
target_link_libraries(RBDLinearizerTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(RBDLinearizerTest "" AUTO)

add_executable(EEKinematicsTest test/robot/kinematics/EEKinematicsTest.cpp)
target_link_libraries(EEKinematicsTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(EEKinematicsTest "" AUTO)

add_executable(EEContactModelTest test/physics/EEContactModelTest.cpp)
target_link_libraries(EEContactModelTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(EEContactModelTest "" AUTO)

add_executable(TaskSpaceCfTest test/robot/costfunction/TaskspaceCostFunctionTest.cpp)
# hack to allow include "../../../ct_optcon/test/costfunction/compareCostFunctions.h"
target_include_directories(TaskSpaceCfTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test/robot/costfunction/../")
target_link_libraries(TaskSpaceCfTest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(TaskSpaceCfTest "" AUTO)

add_executable(rbdJITtests test/robot/costfunction/rbdJITtests.cpp)
target_link_libraries(rbdJITtests ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(rbdJITtests "" AUTO)

add_executable(kindrJITtest test/robot/costfunction/kindrJITtest.cpp)
target_link_libraries(kindrJITtest ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(kindrJITtest "" AUTO)

add_executable(jacobianTests test/robot/jacobian/JacobianTests.cpp)
target_link_libraries(jacobianTests ct_rbd ${GTEST_LIBRARY})
gtest_add_tests(jacobianTests "" AUTO)

install(TARGETS ct_rbd DESTINATION lib/ct)
install (
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION include
    FILES_MATCHING 
    PATTERN "*.hpp"
    PATTERN "*.h"
    REGEX "/[^.]+$" # no extension
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/ct_rbd.doxyfile)
  set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target("doc_${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
    COMMAND ${CMAKE_COMMAND} -E echo_append "API Documentation built in ${CMAKE_CURRENT_SOURCE_DIR}/doc"
    VERBATIM)
endif()

# ct_configure_clang_tidy(INC_DIRS)
