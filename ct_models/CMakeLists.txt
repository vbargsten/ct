project(ct_models)

find_package(Boost REQUIRED system filesystem)

set(ct_model_libs
    quadrotorDynamics
    HyALinearizedForward
    HyAJacInverseDynamicsReverse
    hya_ik
    irb4600_ik
    InvertedPendulumActDynLinearizedForward
)

if(BUILD_HYQ_FULL)
  set(ct_model_libs ${ct_model_libs} HyQWithContactModelLinearizedForward HyQForwardZero)
endif(BUILD_HYQ_FULL)

## assemble list of libraries that contain prespecified templates
if(USE_PRESPEC)
    ct_configure_explicit_templates("${CMAKE_CURRENT_SOURCE_DIR}/../ct/config/explicit_templates.cfg" "${CMAKE_CURRENT_SOURCE_DIR}/prespec/" "ct_models")
    message(WARNING "CT Models: Compiling the following explict template libraries: ${PRESPEC_LIB_NAMES}")
endif(USE_PRESPEC)

# catkin_package(
#   INCLUDE_DIRS
#     include
#   LIBRARIES
#     ${ct_model_libs}
#     ${PRESPEC_LIB_NAMES}
#   CATKIN_DEPENDS
#     ct_rbd
# )

# include_directories(
#   include
#   ${catkin_INCLUDE_DIRS}
# )
add_library(ct_models INTERFACE)
target_include_directories(ct_models INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(ct_models INTERFACE ct_core ct_optcon ct_rbd)

set(IP_CODEGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/models/InvertedPendulum/codegen")
set(HYA_CODEGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/models/HyA/codegen")
set(HYQ_CODEGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/models/HyQ/codegen")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/ct/models/CodegenOutputDirs.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ct/models/CodegenOutputDirs.h)

# add libraries for explicit template
if(USE_PRESPEC)
    ct_add_explicit_template_libs()
endif(USE_PRESPEC)

################ HyQ #################
add_executable(HyQLinearizationCodgen src/HyQ/codegen/HyQLinearizationCodgen.cpp)
target_link_libraries(HyQLinearizationCodgen ct_models )

if (BUILD_HYQ_FULL)
  if (NOT USE_CLANG)
    MESSAGE(WARNING "HyQ Linearization should be build with CLANG")
  endif()
    add_library(HyQWithContactModelLinearizedForward include/ct/models/HyQ/codegen/HyQWithContactModelLinearizedForward.cpp)
    target_link_libraries(HyQWithContactModelLinearizedForward ct_models )

    ## Forward Dynamics Forward Zero
    add_library(HyQForwardZero include/ct/models/HyQ/codegen/HyQForwardZero.cpp)
    target_link_libraries(HyQForwardZero ct_models)
endif(BUILD_HYQ_FULL)

if (BUILD_HYQ_LINEARIZATION_TIMINGS)
  if (NOT USE_CLANG)
    MESSAGE(FATAL_ERROR "HyQ Linearization Timings need to be build with CLANG")
  endif()

  ## Forward Dynamics
  add_library(HyQWithContactModelLinearizedReverse include/ct/models/HyQ/codegen/HyQWithContactModelLinearizedReverse.cpp)
  target_link_libraries(HyQWithContactModelLinearizedReverse ct_models )
  add_library(HyQBareModelLinearizedForward include/ct/models/HyQ/codegen/HyQBareModelLinearizedForward.cpp)
  target_link_libraries(HyQBareModelLinearizedForward ct_models )
  add_library(HyQBareModelLinearizedReverse include/ct/models/HyQ/codegen/HyQBareModelLinearizedReverse.cpp)
  target_link_libraries(HyQBareModelLinearizedReverse ct_models )

  ## Inverse Dynamics
  add_library(HyQJacInverseDynamicsForward include/ct/models/HyQ/codegen/HyQInverseDynJacForward.cpp)
  target_link_libraries(HyQJacInverseDynamicsForward ct_models )
  add_library(HyQJacInverseDynamicsReverse include/ct/models/HyQ/codegen/HyQInverseDynJacReverse.cpp)
  target_link_libraries(HyQJacInverseDynamicsReverse ct_models )

  ## ForwardKinematics
  add_library(HyQJacForwardKinForward include/ct/models/HyQ/codegen/HyQForwardKinJacForward.cpp)
  target_link_libraries(HyQJacForwardKinForward ct_models )
  add_library(HyQJacForwardKinReverse include/ct/models/HyQ/codegen/HyQForwardKinJacReverse.cpp)
  target_link_libraries(HyQJacForwardKinReverse ct_models )


  add_executable(HyQcompareForwardReverseFD src/HyQ/codegen/compareForwardReverseFD.cpp)
  target_link_libraries(HyQcompareForwardReverseFD
                        HyQWithContactModelLinearizedForward
                        HyQWithContactModelLinearizedReverse
                        HyQBareModelLinearizedForward
                        HyQBareModelLinearizedReverse
                        ct_models)

  add_executable(HyQcompareForwardReverseID src/HyQ/codegen/compareForwardReverseID.cpp)
  target_link_libraries(HyQcompareForwardReverseID
                        HyQJacInverseDynamicsForward
                        HyQJacInverseDynamicsReverse
                        ct_models)

  add_executable(HyQcompareForwardReverseKin src/HyQ/codegen/compareForwardReverseKin.cpp)
  target_link_libraries(HyQcompareForwardReverseKin
                        HyQJacForwardKinForward
                        HyQJacForwardKinReverse
                        ct_models)

    add_executable(HyQcompareForwardZero src/HyQ/codegen/compareForwardZero.cpp)
    target_link_libraries(HyQcompareForwardZero
                          HyQForwardZero
                          ct_models)
endif(BUILD_HYQ_LINEARIZATION_TIMINGS)

########## Inverted Pendulum #########
#add_executable(InvertedPendulumWithActuatorCodeGen src/InvertedPendulum/codegen/InvertedPendulumWithActuatorCodeGen.cpp)
#target_link_libraries(InvertedPendulumWithActuatorCodeGen ct_models)

add_library(InvertedPendulumActDynLinearizedForward include/ct/models/InvertedPendulum/codegen/InvertedPendulumActDynLinearizedForward.cpp)
target_link_libraries(InvertedPendulumActDynLinearizedForward ct_models)


################ HyA #################
#add_executable(HyALinearizationCodegen src/HyA/codegen/HyALinearizationCodeGen.cpp)
#target_link_libraries(HyALinearizationCodegen ct_models)

add_library(HyALinearizedForward include/ct/models/HyA/codegen/HyALinearizedForward.cpp)
target_link_libraries(HyALinearizedForward ct_models )

add_library(HyAJacInverseDynamicsReverse include/ct/models/HyA/codegen/HyAInverseDynJacReverse.cpp)
target_link_libraries(HyAJacInverseDynamicsReverse ct_models )

if(BUILD_HYA_LINEARIZATION_TIMINGS)
  add_library(HyALinearizedReverse include/ct/models/HyA/codegen/HyALinearizedReverse.cpp)
  target_link_libraries(HyALinearizedReverse ct_models )

  add_library(HyAJacInverseDynamicsForward include/ct/models/HyA/codegen/HyAInverseDynJacForward.cpp)
  target_link_libraries(HyAJacInverseDynamicsForward ct_models )

  add_executable(HyAcompareForwardReverse src/HyA/codegen/compareForwardReverse.cpp)
  target_link_libraries(HyAcompareForwardReverse
                        HyALinearizedForward
                        HyALinearizedReverse
                        HyAJacInverseDynamicsForward
                        HyAJacInverseDynamicsReverse
                        ct_models)
endif(BUILD_HYA_LINEARIZATION_TIMINGS)


################ Quadrotor #################
## Declare a cpp library for the ordinary quadrotor
add_library(quadrotorDynamics
            src/Quadrotor/quadrotor_ode.cpp
            src/Quadrotor/A_quadrotor.cpp
            src/Quadrotor/B_quadrotor.cpp
            src/Quadrotor/C_quadrotor.cpp)
# getting the INTERFACE_INCLUDE_DIRECTORIES only won't work, so using this
# to add the rbd include dirs. ehhh...

# catkin_add_gtest(QuadrotorWithLoadTest test/QuadrotorWithLoad/QuadrotorWithLoadTest.cpp)
# target_link_libraries(QuadrotorWithLoadTest
#                       ct_models)
target_link_libraries(quadrotorDynamics PRIVATE ct_models)


## Unit tests
# catkin_add_gtest(HyaTest test/HyA/HyATest.cpp)
# target_link_libraries(HyaTest
#                       ct_models
#                       HyALinearizedForward)

################ Inverse Kinematics #################
add_library(hya_ik
  src/HyA/transform6d.cpp
)
set_target_properties(hya_ik PROPERTIES COMPILE_FLAGS "-fPIC -DIKFAST_NAMESPACE=hya_ik -DIKFAST_NO_MAIN -Wno-unused-variable")

# getting the INTERFACE_INCLUDE_DIRECTORIES only won't work, so using this
# to add the rbd include dirs. ehhh...
target_link_libraries(hya_ik PRIVATE ct_rbd)

add_library(irb4600_ik
  src/Irb4600/transform6d.cpp
)
# get_property(
#     ct_rbd_include_dirs
#     TARGET ct_models
#     INTERFACE_INCLUDE_DIRECTORIES
# )
# message("BLABLA: ${ct_rbd_include_dirs}")
# target_include_directories(irb4600_ik $<BUILD_INTERFACE:$<TARGET_PROPERTY:ct_models,INTERFACE_INCLUDE_DIRECTORIES>>})
set_target_properties(irb4600_ik PROPERTIES COMPILE_FLAGS "-fPIC -DIKFAST_NAMESPACE=irb4600_ik -DIKFAST_NO_MAIN -Wno-unused-variable")

# getting the INTERFACE_INCLUDE_DIRECTORIES only won't work, so using this
# to add the rbd include dirs. ehhh...
target_link_libraries(irb4600_ik PRIVATE ct_rbd)

if(BUILD_EXAMPLES)
    message(STATUS "Building with examples.")

    set(CT_MODELS_EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/examples/exampleDir.h.in ${CMAKE_CURRENT_SOURCE_DIR}/examples/exampleDir.h)

    add_executable(ex_NLOC_MPC_invertedPendulum examples/mpc/InvertedPendulum/NLOC_MPC.cpp)
    target_link_libraries(ex_NLOC_MPC_invertedPendulum ct_models)
endif()

# Tests.
# catkin_add_gtest(ikfast_test test/IKFast/IKFastTest.cpp)
# target_link_libraries(ikfast_test
#                       hya_ik
#                       irb4600_ik
#                       ct_models)


find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/ct_models.doxyfile)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
        COMMAND ${CMAKE_COMMAND} -E echo_append "API Documentation built in ${CMAKE_CURRENT_SOURCE_DIR}/doc"
        VERBATIM)
endif()
