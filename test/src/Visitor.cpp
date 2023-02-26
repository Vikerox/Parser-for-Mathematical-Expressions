#include <gtest/gtest.h>
#include <pfme/Visitor.hpp>

TEST ( Visitor, visit )
{
    std::unique_ptr<pfme::Parser> parser_1 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5" ) ) );
    pfme::Visitor                 visitor_1 ( std::move ( parser_1 ) );

    std::unique_ptr<pfme::Parser> parser_2 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5+5" ) ) );
    pfme::Visitor                 visitor_2 ( std::move ( parser_2 ) );

    std::unique_ptr<pfme::Parser> parser_3 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5/5" ) ) );
    pfme::Visitor                 visitor_3 ( std::move ( parser_3 ) );

    std::unique_ptr<pfme::Parser> parser_4 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5+ 3.8" ) ) );
    pfme::Visitor                 visitor_4 ( std::move ( parser_4 ) );

    std::unique_ptr<pfme::Parser> parser_5 (
        std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5 * 3 * 3 + ( ( 4.3 + 3 * 5 ) + 24 ) * 3 / 7" ) ) );
    pfme::Visitor visitor_5 ( std::move ( parser_5 ) );

    ASSERT_EQ ( visitor_1.visit(), std::to_string ( 5 ) );
    ASSERT_EQ ( visitor_2.visit(), std::to_string ( 5 + 5 ) );
    ASSERT_EQ ( visitor_3.visit(), std::to_string ( 5 / 5 ) );
    ASSERT_EQ ( visitor_4.visit(), std::to_string ( 5 + 3.8 ) );
    ASSERT_EQ ( visitor_5.visit(), std::to_string ( 5 * 3 * 3 + ( ( 4.3 + 3 * 5 ) + 24 ) * 3 / 7 ) );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
