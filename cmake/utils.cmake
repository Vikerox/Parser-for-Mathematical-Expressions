#
# Add a target for formating the project using `clang-format` (i.e: cmake --build build --target clang-format)
#

function(add_clang_format_target)
    if(NOT ${PROJECT_NAME}_CLANG_FORMAT_BINARY)
			find_program(${PROJECT_NAME}_CLANG_FORMAT_BINARY clang-format)
    endif()

    if(${PROJECT_NAME}_CLANG_FORMAT_BINARY)
		add_custom_target(clang-format
				COMMAND ${${PROJECT_NAME}_CLANG_FORMAT_BINARY}
				-i ${exe_sources} ${headers} ${source_test_sources}
				WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
		message(STATUS "Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format)./n")
    endif()
endfunction()

function(add_clang_tidy_target)
	find_program(CLANGTIDY clang-tidy)
	set(CMAKE_CXX_CLANG_TIDY 
		"${CLANGTIDY};--extra-arg=-Wno-unknown-warning-option;--extra-arg=-Xclang;--extra-arg=-fcxx-exceptions;--extra-arg=-std=c++20;--extra-arg=-I${CMAKE_CURRENT_SOURCE_DIR}/include;--extra-arg=-DLIBCXX_ENABLE_INCOMPLETE_FEATURES=ON;--extra-arg=-stdlib=libc++;--extra-arg=-fexperimental-library")
	add_custom_target(
        clang-tidy
        COMMAND ${CMAKE_CXX_CLANG_TIDY}
        ${absolute_exe_sources}
        ${absolute_headers}
)

endfunction()

function(add_cppcheck_target)
	find_program(CPPCHECK cppcheck)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
                           --inline-suppr --inconclusive)
	add_custom_target(
		cppckeck
		COMMAND ${CMAKE_CXX_CPPCHECK}
        ${absolute_exe_sources}
        ${absolute_headers}
	)
endfunction()