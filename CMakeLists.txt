# CMakeList.txt : CMake project for CMake Mathe Parser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)
set(CMAKE_FIND_DEBUG_MODE 1)

project (
	Parser-for-Mathematical-Expressions
	VERSION 0.1.0
	LANGUAGES CXX
)

set(PROJECT_NAME_SHORT "pfme")

if(EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
	message("Setting up vcpkg")
	include( "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
	if(MSVC)
		set(VCPKG_TARGET_TRIPLET "x64-windows")
	elseif(UNIX)
		set(VCPKG_TARGET_TRIPLET "x64-linux")
	else()
		message("Unknown operating system for vcpkg cinfiguration")
	endif()
endif()

include(cmake/utils.cmake)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

#
# Prevent building in the source directory
#

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

#
# Add source to this project's executable.
#

include(cmake/filepaths.cmake)

add_executable(${PROJECT_NAME} ${exe_sources})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME_SHORT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

include(cmake/CompilerWarnings.cmake)

set_project_warnings(${PROJECT_NAME})


# find_package(fmt CONFIG REQUIRED)

# target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

#find_package(pdcurses)

#find_path(CURSES_INCLUDE_DIRS curses.h)
#find_library(CURSES_LIBRARYS pdcurses)
#include_directories(${CURSES_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARYS})

#
# Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format)
#

add_clang_format_target()

#
# Install library for easy downstream inclusion
#

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

#
# Set the build/user include directories
#

# Allow usage of header files in the `src` directory, but only for utilities

target_include_directories(
    ${PROJECT_NAME}
	PUBLIC
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

#
# Static Analysis tools
#

add_clang_tidy_target()

#if(${PROJECT_NAME}_ENABLE_CPPCHECK)
#  find_program(CPPCHECK cppcheck)
#  if(CPPCHECK)
#    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
#                           --inline-suppr --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
#    message("Cppcheck finished setting up.")
#  else()
#    message(SEND_ERROR "Cppcheck requested but executable not found.")
#  endif()
# endif()

#
# CCache
#

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

#
# Doxygen
#
option(${PROJECT_NAME}_ENABLE_DOXYGEN "Enable Doxygen documentation builds of source." ON)


if(${PROJECT_NAME}_ENABLE_DOXYGEN)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)

    find_package(Doxygen REQUIRED dot)
    doxygen_add_docs(doxygen-docs ${PROJECT_SOURCE_DIR})

    message("Doxygen has been setup and documentation is now available.")
endif()

#
# tests
#

add_library(${PROJECT_NAME}_LIB ${absolute_headers} ${absolute_sources})

set_target_properties(
${PROJECT_NAME}_LIB
PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
OUTPUT_NAME ${PROJECT_NAME}
)

target_compile_features(${PROJECT_NAME}_LIB PUBLIC cxx_std_20)

target_include_directories(
	${PROJECT_NAME}_LIB
	PUBLIC
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
	${PROJECT_NAME}_LIB
	PUBLIC
	# fmt::fmt-header-only
)

enable_testing()
add_subdirectory(test)