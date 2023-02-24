#include <gtest/gtest.h>
#include <pfme/Parser.hpp>

TEST ( Parser, parse )
{
    pfme::Lexer  lex_1 ( "5" );
    pfme::Parser parser_1 ( std::make_unique<pfme::Lexer> ( lex_1 ) );
    pfme::Lexer  lex_2 ( "5+6/2" );
    pfme::Parser parser_2 ( std::make_unique<pfme::Lexer> ( lex_2 ) );
    pfme::Lexer  lex_3 ( "5/6+2" );
    pfme::Parser parser_3 ( std::make_unique<pfme::Lexer> ( lex_3 ) );
    pfme::Lexer  lex_4 ( "5*(2+2)" );
    pfme::Parser parser_4 ( std::make_unique<pfme::Lexer> ( lex_4 ) );

    ASSERT_EQ ( parser_1.parse()->m_type, pfme::AST_TYPE::INTEGER );
    ASSERT_EQ ( parser_2.parse()->m_type, pfme::AST_TYPE::ADDITION );
    ASSERT_EQ ( parser_3.parse()->m_type, pfme::AST_TYPE::ADDITION );
    ASSERT_EQ ( parser_4.parse()->m_type, pfme::AST_TYPE::MULTIPLICATION );
}

TEST ( Parser, printBT )
{
    pfme::Lexer  lex_1 ( "5" );
    pfme::Parser parser_1 ( std::make_unique<pfme::Lexer> ( lex_1 ) );

    pfme::Lexer  lex_2 ( "5*(2+2)" );
    pfme::Parser parser_2 ( std::make_unique<pfme::Lexer> ( lex_2 ) );

    testing::internal::CaptureStdout();
    pfme::Parser::print_binary_tree ( parser_1.parse().get() );
    auto output_1 = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    pfme::Parser::print_binary_tree ( parser_2.parse().get() );
    auto output_2 = testing::internal::GetCapturedStdout();

    ASSERT_EQ ( output_1, "└──5\n\n" );
    ASSERT_EQ ( output_2.starts_with ( "└──*\n" ), true );
}


int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
