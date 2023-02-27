#include <gtest/gtest.h>
#include <pfme/Fraction.hpp>

using LD  = long double;
using LLI = long long int;

TEST ( Fraction, constructor )
{
    pfme::Fraction frac1 { 5 };
    pfme::Fraction frac2 { 5, 1 };

    ASSERT_EQ ( frac1, frac2 );
}


int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}