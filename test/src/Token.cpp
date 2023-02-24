#include <gtest/gtest.h>
#include <pfme/Token.hpp>

TEST ( Token, Token )
{
    pfme::Token token_tok ( pfme::TOKEN_TYPE::TOKEN_ADDITION, "+" );
    pfme::Token token_char ( '-', "-" );

    ASSERT_EQ ( token_tok.get_type(), pfme::TOKEN_TYPE::TOKEN_ADDITION );
    ASSERT_EQ ( token_tok.get_value(), "+" );
    ASSERT_EQ ( pfme::token_type_to_string ( token_tok.get_type() ), "TOKEN_ADDITION" );

    ASSERT_EQ ( token_char.get_type(), pfme::TOKEN_TYPE::TOKEN_SUBTRACTION );
    ASSERT_EQ ( token_char.get_value(), "-" );
    ASSERT_EQ ( pfme::token_type_to_string ( token_char.get_type() ), "TOKEN_SUBTRACTION" );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
