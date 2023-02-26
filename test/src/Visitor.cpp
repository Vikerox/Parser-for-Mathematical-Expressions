#include <gtest/gtest.h>
#include <pfme/Visitor.hpp>

TEST ( Visitor, visit )
{
    std::unique_ptr<pfme::Parser> parser_1 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5" ) ) );
    parser_1->parse();
    pfme::Visitor visitor_1 ( std::move ( parser_1 ) );

    std::unique_ptr<pfme::Parser> parser_2 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5+5" ) ) );
    parser_2->parse();
    pfme::Visitor visitor_2 ( std::move ( parser_2 ) );

    std::unique_ptr<pfme::Parser> parser_3 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5/5" ) ) );
    parser_3->parse();
    pfme::Visitor visitor_3 ( std::move ( parser_3 ) );

    std::unique_ptr<pfme::Parser> parser_4 ( std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5+ 3.8" ) ) );
    parser_4->parse();
    pfme::Visitor visitor_4 ( std::move ( parser_4 ) );

    ASSERT_EQ ( visitor_1.visit(), std::to_string ( 5 ) );
    ASSERT_EQ ( visitor_2.visit(), std::to_string ( 5 + 5 ) );
    ASSERT_EQ ( visitor_3.visit(), std::to_string ( 5 / 5 ) );
    ASSERT_EQ ( visitor_4.visit(), std::to_string ( 5 + 3.8 ) );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
