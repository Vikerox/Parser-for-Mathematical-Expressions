#include <gtest/gtest.h>
#include <pfme/Visitor.hpp>

TEST ( Visitor, visit )
{
    pfme::Visitor visitor_1 ( "5" );

    pfme::Visitor visitor_2 ( "5+5" );

    pfme::Visitor visitor_3 ( "5/5" );

    pfme::Visitor visitor_4 ( "5+ 3.8" );

    std::unique_ptr<pfme::Parser> parser_5 (
        std::make_unique<pfme::Parser> ( std::make_unique<pfme::Lexer> ( "5 * 3 * 3 + ( ( 4.3 + 3 * 5 ) + 24 ) * 3 / 7" ) ) );
    pfme::Visitor visitor_5 ( std::move ( parser_5 ) );

    ASSERT_EQ ( visitor_1.visit(), std::to_string ( 5 ) );
    ASSERT_EQ ( visitor_2.visit(), std::to_string ( 5 + 5 ) );
    ASSERT_EQ ( visitor_3.visit(), std::to_string ( 5 / 5 ) );
    ASSERT_EQ ( visitor_4.visit(), std::to_string ( 5 + 3.8 ) );
    ASSERT_EQ ( visitor_5.visit(), std::to_string ( 5 * 3 * 3 + ( ( 4.3 + 3 * 5 ) + 24 ) * 3 / 7 ) );
}

TEST ( Visitor, special_sequences )
{
    pfme::Visitor v1 ( "5+" );
    pfme::Visitor v2 ( "5*" );
    pfme::Visitor v3 ( "5)))" );
    pfme::Visitor v4 ( "(((5" );
    pfme::Visitor v5 ( "5/7" );
    pfme::Visitor v6 ( "5/7." );
    pfme::Visitor v7 ( "5/0" );
    pfme::Visitor v8 ( "5/0." );
    pfme::Visitor v9 ( "5./0" );
    pfme::Visitor vA ( "5^0" );
    pfme::Visitor vB ( ".5^0" );
    pfme::Visitor vC ( "2+-5" );


    ASSERT_EQ ( v1.visit(), std::to_string ( 5 + 0 ) );
    ASSERT_EQ ( v2.visit(), std::to_string ( 5 * 0 ) );
    ASSERT_EQ ( v3.visit(), std::to_string ( 5 ) );
    ASSERT_EQ ( v4.visit(), std::to_string ( 5 ) );
    ASSERT_EQ ( v5.visit(), pfme::Fraction ( 5, 7 ).to_string() );
    ASSERT_EQ ( v6.visit(), std::to_string ( 5 / 7. ) );
    ASSERT_ANY_THROW ( v7.visit() );
    ASSERT_ANY_THROW ( v8.visit() );
    ASSERT_ANY_THROW ( v9.visit() );
    ASSERT_EQ ( vA.visit(), std::to_string ( 1 ) );
    ASSERT_EQ ( vB.visit(), std::to_string ( 1 ) );
    ASSERT_EQ ( vC.visit(), std::to_string ( 2 - 5 ) );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
