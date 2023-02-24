#include <gtest/gtest.h>
#include <pfme/Lexer.hpp>

TEST ( Lexer, Constructor )
{
    pfme::Lexer lex ( "5 + 6" );
    ASSERT_EQ ( lex.get_content(), "5 + 6" );
}

TEST ( Lexer, get_next_token )
{
    pfme::Lexer lex ( "5 + (6/0.6)" );

    ASSERT_EQ ( lex.get_next_token()->get_value(), "5" );
    ASSERT_EQ ( lex.get_next_token()->get_type(), pfme::TOKEN_TYPE::TOKEN_ADDITION );
    ASSERT_EQ ( lex.get_next_token()->get_type(), pfme::TOKEN_TYPE::TOKEN_L_PAREN );
    ASSERT_EQ ( lex.get_next_token()->get_value(), "6" );
    ASSERT_EQ ( lex.get_next_token()->get_type(), pfme::TOKEN_TYPE::TOKEN_DIVISION );
    ASSERT_EQ ( lex.get_next_token()->get_value(), "0.6" );
    ASSERT_EQ ( lex.get_next_token()->get_type(), pfme::TOKEN_TYPE::TOKEN_R_PAREN );
    ASSERT_EQ ( lex.get_next_token()->get_type(), pfme::TOKEN_TYPE::TOKEN_EOF );
}

TEST ( Lexer, error_string )
{
    pfme::Lexer lex ( "5 + (6/0.6)" );

    ASSERT_EQ ( lex.error_string ( "test1" ).starts_with ( "test1" ), true );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}