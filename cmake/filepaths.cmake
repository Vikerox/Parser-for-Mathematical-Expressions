set(sources
    src/cpp/AST.cpp
    src/cpp/Lexer.cpp
    src/cpp/Parser.cpp
    src/cpp/Token.cpp
    src/cpp/Visitor.cpp
)

set(absolute_sources ${sources})
list(TRANSFORM absolute_sources PREPEND "/")
list(TRANSFORM absolute_sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR})

set(exe_sources
		src/main/main.cpp
		${sources}
)

set(absolute_exe_sources ${exe_sources})
list(TRANSFORM absolute_exe_sources PREPEND "/")
list(TRANSFORM absolute_exe_sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR})

set(headers
    include/pfme/AST.hpp
    include/pfme/Lexer.hpp
    include/pfme/Parser.hpp
    include/pfme/Token.hpp
    include/pfme/Visitor.hpp
)

set(absolute_headers ${headers})
list(TRANSFORM absolute_headers PREPEND "/")
list(TRANSFORM absolute_headers PREPEND ${CMAKE_CURRENT_SOURCE_DIR})

set(test_sources
	src/AST.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Token.cpp
    src/Visitor.cpp
)

set(source_test_sources ${test_sources})
list(TRANSFORM source_test_sources PREPEND "test/")
