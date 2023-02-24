cmake_minimum_required(VERSION 3.15)

#
# Project details
#

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

message("Adding tests under ${CMAKE_PROJECT_NAME}Tests...")


if(NOT TARGET Tests)
	add_custom_target(Tests)
endif()

foreach(file ${test_sources})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name ${file}) 
  add_executable(Test_${test_name} ${file})

  #
  # Set the compiler standard
  #

  target_compile_features(Test_${test_name} PUBLIC cxx_std_20)
  add_dependencies(Tests Test_${test_name})
  #
  # Setup code coverage if enabled
  #

  #if (${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
  #  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  #  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
  #  verbose_message("Code coverage is enabled and provided with GCC.")
  #endif()

  #
  # Load the desired unit testing framework
  #
  # Currently supported: GoogleTest (and GoogleMock), Catch2.


  set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_LIB)

  
	find_package(GTest REQUIRED)

	target_link_libraries(
		Test_${test_name}
		PRIVATE
		GTest::gtest
		GTest::gtest_main
		${GOOGLE_MOCK_LIBRARIES}
		${${CMAKE_PROJECT_NAME}_TEST_LIB}
		GTest::gmock
		GTest::gmock_main
	)

  #
  # Add the unit tests
  #

  add_test(
    NAME
      ${test_name}
    COMMAND
      Test_${test_name}
  )
endforeach()

message("Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
