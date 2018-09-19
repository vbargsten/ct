project (ct_core)

# find_package(catkin REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS REQUIRED)
find_package(PythonLibs 2.7 EXACT QUIET)
# find_package(Qwt QUIET)
# find_package(Qt4 QUIET)

if(QWT_FOUND AND Qt4_FOUND)
    #set(QWT_ENABLED true)
    #add_definitions( -DQWT_ENABLED )
    #include(${QT_USE_FILE})
    #include_directories(${QWT_INCLUDE_DIR}})
    #message(STATUS "COMPILING WITH QWT")
    #set(ct_core_extra_libs ${QWT_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
    set(qwt_plotting TRUE)
else()
    message(STATUS "COMPILING WITHOUT QWT...")
    if(NOT QWT_FOUND)
        message(STATUS "    ...QWT not found")
    endif()
    if(NOT Qt4_FOUND)
        message(STATUS "    ...Qt4 not found")
    endif()
    #set(ct_core_extra_libs "")    
    set(qwt_plotting FALSE)
endif()




if (PYTHONLIBS_FOUND)
   message(STATUS "Python found")
   #include_directories(${PYTHON_INCLUDE_DIRS})
   #add_definitions(-DPLOTTING_ENABLED)
   set(python_plotting TRUE)
else()
   message(STATUS "Python not found")
   #set(PYTHON_LIBRARY "")
   set(python_plotting FALSE)
endif()

# include_directories(
#   ${BOOST_INCLUDE_DIRS}
# )


## dummy prespec libs
set(PRESPEC_LIB_NAMES "")

## assemble list of libraries that contain prespecified templates
if(USE_PRESPEC)
    ct_configure_explicit_templates("${CMAKE_CURRENT_SOURCE_DIR}/../ct/config/explicit_templates.cfg" "${CMAKE_CURRENT_SOURCE_DIR}/prespec/" "ct_core")
    message(WARNING "CT Core: Compiling the following explict template libraries: ${PRESPEC_LIB_NAMES}")
endif(USE_PRESPEC)

#used later for clang-tidy
#set(INC_DIRS ${BOOST_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/external" ${EIGEN3_INCLUDE_DIR})

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
   src/core/plot/plot.cpp
)
target_include_directories(ct_core PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/" 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ct/external" 
    ${EIGEN3_INCLUDE_DIR}
)


# add libraries for explicit templates
target_link_libraries(ct_core PUBLIC
    ${CMAKE_DL_LIBS}
)

if (qwt_plotting) 
    include(${QT_USE_FILE})
    target_compile_definitions(ct_core PUBLIC "QWT_ENABLED")
    target_include_directories(ct_core PUBLIC "${QWT_INCLUDE_DIR}")
    target_link_libraries(ct_core PUBLIC ${QWT_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
endif()

if (python_plotting)
    target_include_directories(ct_core PUBLIC ${PYTHON_INCLUDE_DIRS})
    target_compile_definitions(ct_core PUBLIC "PLOTTING_ENABLED")
    target_link_libraries(ct_core PUBLIC ${PYTHON_LIBRARY})
endif()



if(USE_PRESPEC)
    ct_add_explicit_template_libs()
endif(USE_PRESPEC)


if(BUILD_EXAMPLES)
    add_executable(ex_DampedOscillatorCustomController examples/DampedOscillatorCustomController.cpp)
    target_link_libraries(ex_DampedOscillatorCustomController ct_core)
    
    add_executable(ex_DampedOscillator examples/DampedOscillator.cpp)
    target_link_libraries(ex_DampedOscillator ct_core)
    
    add_executable(ex_MasspointIntegration examples/MasspointIntegration.cpp)
    target_link_libraries(ex_MasspointIntegration ct_core)
endif()
# 
# catkin_add_gtest(NoiseTest test/NoiseTest.cpp)
# target_link_libraries(NoiseTest ${catkin_LIBRARIES})
add_executable(NoiseTest test/NoiseTest.cpp)
target_link_libraries(NoiseTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(NoiseTest "" AUTO)
# 
# catkin_add_gtest(SecondOrderSystemTest test/SecondOrderSystemTest.cpp)
# target_link_libraries(SecondOrderSystemTest ${catkin_LIBRARIES})
add_executable(SecondOrderSystemTest test/SecondOrderSystemTest.cpp)
target_link_libraries(SecondOrderSystemTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SecondOrderSystemTest "" AUTO)
# 
# # This test is not required to link against ${PRESPEC_LIB_NAMES} as it is not using prespec. Here we ensure that linking against it does not fail.
# catkin_add_gtest(IntegrationTest test/integration/IntegrationTest.cpp)
# target_link_libraries(IntegrationTest ${catkin_LIBRARIES} ct_core ${PRESPEC_LIB_NAMES})
add_executable(IntegrationTest test/integration/IntegrationTest.cpp)
target_link_libraries(IntegrationTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(IntegrationTest "" AUTO)
# 
# catkin_add_gtest(IntegratorComparison test/integration/IntegratorComparison.cpp)
# target_link_libraries(IntegratorComparison ${catkin_LIBRARIES} ct_core)
add_executable(IntegratorComparison test/integration/IntegratorComparison.cpp)
target_link_libraries(IntegratorComparison ct_core ${GTEST_LIBRARY})
gtest_add_tests(IntegratorComparison "" AUTO)
# 
# catkin_add_gtest(SymplecticIntegrationTest test/integration/SymplecticIntegrationTest.cpp)
# target_link_libraries(SymplecticIntegrationTest ${catkin_LIBRARIES} ct_core)
add_executable(SymplecticIntegrationTest test/integration/SymplecticIntegrationTest.cpp)
target_link_libraries(SymplecticIntegrationTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SymplecticIntegrationTest "" AUTO)
# 
# catkin_add_gtest(SystemDiscretizerTest test/integration/SystemDiscretizerTest.cpp)
# target_link_libraries(SystemDiscretizerTest ${catkin_LIBRARIES} ct_core)
add_executable(SystemDiscretizerTest test/integration/SystemDiscretizerTest.cpp)
target_link_libraries(SystemDiscretizerTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SystemDiscretizerTest "" AUTO)
# 
# add_executable(SensitivityTest test/integration/sensitivity/SensitivityTest.cpp)
# target_link_libraries(SensitivityTest ${catkin_LIBRARIES} ct_core)
add_executable(SensitivityTest test/integration/sensitivity/SensitivityTest.cpp)
target_link_libraries(SensitivityTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SensitivityTest "" AUTO)
# 
# catkin_add_gtest(InterpolationTest test/InterpolationTest.cpp)
# target_link_libraries(InterpolationTest ${catkin_LIBRARIES})
add_executable(InterpolationTest test/InterpolationTest.cpp)
target_link_libraries(InterpolationTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(InterpolationTest "" AUTO)
# 
# catkin_add_gtest(DiscreteArrayTest test/DiscreteArrayTest.cpp)
# target_link_libraries(DiscreteArrayTest ${catkin_LIBRARIES})
add_executable(DiscreteArrayTest test/DiscreteArrayTest.cpp)
target_link_libraries(DiscreteArrayTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(DiscreteArrayTest "" AUTO)
# 
# catkin_add_gtest(DiscreteTrajectoryTest test/DiscreteTrajectoryTest.cpp)
# target_link_libraries(DiscreteTrajectoryTest ${catkin_LIBRARIES})
add_executable(DiscreteTrajectoryTest test/DiscreteTrajectoryTest.cpp)
target_link_libraries(DiscreteTrajectoryTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(DiscreteTrajectoryTest "" AUTO)
# 
# catkin_add_gtest(LinspaceTest test/LinspaceTest.cpp)
# target_link_libraries(LinspaceTest ${catkin_LIBRARIES})
add_executable(LinspaceTest test/LinspaceTest.cpp)
target_link_libraries(LinspaceTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(LinspaceTest "" AUTO)
# 
# catkin_add_gtest(AutoDiffLinearizerTest test/AutoDiffLinearizerTest.cpp)
# target_link_libraries(AutoDiffLinearizerTest ${catkin_LIBRARIES} ct_core pthread)
add_executable(AutoDiffLinearizerTest test/AutoDiffLinearizerTest.cpp)
target_link_libraries(AutoDiffLinearizerTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(AutoDiffLinearizerTest "" AUTO)
# 
# catkin_add_gtest(SwitchingTest test/switching/SwitchingTest.cpp)
# target_link_libraries(SwitchingTest ${catkin_LIBRARIES} ct_core)
add_executable(SwitchingTest test/switching/SwitchingTest.cpp)
target_link_libraries(SwitchingTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SwitchingTest "" AUTO)
# 
# catkin_add_gtest(SwitchedControlledSystemTest test/switching/SwitchedControlledSystemTest.cpp)
# target_link_libraries(SwitchedControlledSystemTest ${catkin_LIBRARIES} ct_core)
add_executable(SwitchedControlledSystemTest test/switching/SwitchedControlledSystemTest.cpp)
target_link_libraries(SwitchedControlledSystemTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SwitchedControlledSystemTest "" AUTO)
# 
# catkin_add_gtest(SwitchedDiscreteControlledSystemTest test/switching/SwitchedDiscreteControlledSystemTest.cpp)
# target_link_libraries(SwitchedDiscreteControlledSystemTest ${catkin_LIBRARIES} ct_core)
add_executable(SwitchedDiscreteControlledSystemTest test/switching/SwitchedDiscreteControlledSystemTest.cpp)
target_link_libraries(SwitchedDiscreteControlledSystemTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(SwitchedDiscreteControlledSystemTest "" AUTO)
# 
# catkin_add_gtest(MatrixInversionTest test/math/MatrixInversionTest.cpp)
# target_link_libraries(MatrixInversionTest ${catkin_LIBRARIES})
add_executable(MatrixInversionTest test/math/MatrixInversionTest.cpp)
target_link_libraries(MatrixInversionTest ct_core ${GTEST_LIBRARY})
gtest_add_tests(MatrixInversionTest "" AUTO)
# 
# # Codegen tests cannot run in parallel. Thus we build and run all codegen tests in one
# catkin_add_gtest(CodegenTests test/CodegenTests.cpp)
# target_link_libraries(CodegenTests ${catkin_LIBRARIES} ct_core dl pthread)
add_executable(CodegenTests test/CodegenTests.cpp)
target_link_libraries(CodegenTests ct_core ${GTEST_LIBRARY})
gtest_add_tests(CodegenTests "" AUTO)
# 
# ## example for using the build-in plotting tool
add_executable(plotTest test/plot/plotTest.cpp)
target_link_libraries(plotTest ct_core)
# 
if(qwt_plotting)
    add_executable(plotQwtTest test/plot/plotQwtTest.cpp)
    target_link_libraries(plotQwtTest ct_core pthread)
endif()
# 
# 
# ## unit tests for prespec templates
if(USE_PRESPEC)
    #catkin_add_gtest(IntegrationTestPrespec test/integration/IntegrationTestPrespec.cpp)
    #target_link_libraries(IntegrationTestPrespec ${catkin_LIBRARIES} ct_core ${PRESPEC_LIB_NAMES})
    add_executable(IntegrationTestPrespec test/integration/IntegrationTestPrespec.cpp)
    target_link_libraries(IntegrationTestPrespec ct_core ${GTEST_LIBRARY} ${PRESPEC_LIB_NAMES})
    gtest_add_tests(IntegrationTestPrespec "" AUTO)

    #catkin_add_gtest(IntegratorComparisonPrespec test/integration/IntegratorComparisonPrespec.cpp)
    #target_link_libraries(IntegratorComparisonPrespec ${catkin_LIBRARIES} ct_core ${PRESPEC_LIB_NAMES})
    add_executable(IntegratorComparisonPrespec test/integration/IntegratorComparisonPrespec.cpp)
    target_link_libraries(IntegratorComparisonPrespec ct_core ${GTEST_LIBRARY} ${PRESPEC_LIB_NAMES})
    gtest_add_tests(IntegratorComparisonPrespec "" AUTO)
endif(USE_PRESPEC)


install(TARGETS ct_core DESTINATION lib/ct)
install (
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION include
    FILES_MATCHING 
    PATTERN "*.hpp"
    PATTERN "*.h"
    REGEX "/[^.]+$" # no extension
)

## build doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/ct_core.doxyfile)
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
