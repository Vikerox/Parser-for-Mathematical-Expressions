#include <gtest/gtest.h>
#include <pfme/AST.hpp>

TEST ( AST, Constructor )
{
    pfme::AST ast_std;
    pfme::AST ast_t_and_v_i ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_t_and_v_f ( pfme::AST_TYPE::FLOAT, "3.6" );
    pfme::AST ast_node ( std::make_shared<pfme::AST> ( ast_std ) );
    pfme::AST ast_float ( 2.37l );
    pfme::AST ast_integer ( 5ll );

    ASSERT_EQ ( ast_std.m_type, pfme::AST_TYPE::EMPTY );
    ASSERT_EQ ( ast_std.m_operation_level, 0 );
    ASSERT_EQ ( ast_std.m_value, "0" );

    ASSERT_EQ ( ast_t_and_v_i.m_type, pfme::AST_TYPE::INTEGER );
    ASSERT_EQ ( ast_t_and_v_i.m_value, "200" );
    ASSERT_EQ ( std::get<long long int> ( ast_t_and_v_i.m_number ), 200 );

    ASSERT_EQ ( ast_t_and_v_f.m_type, pfme::AST_TYPE::FLOAT );
    ASSERT_EQ ( ast_t_and_v_f.m_value, "3.6" );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ast_t_and_v_f.m_number ), 3.6 );

    ASSERT_EQ ( ast_node.m_type, pfme::AST_TYPE::EMPTY );
    ASSERT_EQ ( ast_node.rhand, nullptr );
    ASSERT_NE ( ast_node.lhand, nullptr );

	ASSERT_EQ ( ast_float.m_type, pfme::AST_TYPE::FLOAT );
    ASSERT_EQ ( ast_float.m_value, std::to_string ( 2.37l ) );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ast_float.m_number ), 2.37l );

    ASSERT_EQ ( ast_integer.m_type, pfme::AST_TYPE::INTEGER );
    ASSERT_EQ ( ast_integer.m_value, std::to_string ( 5 ) );
    ASSERT_EQ ( std::get<long long int> ( ast_integer.m_number ), 5 );
}

TEST ( AST, operation_to_string )
{
    pfme::AST ast_empty ( pfme::AST_TYPE::EMPTY, "empty" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_add ( pfme::AST_TYPE::ADDITION, "+" );

    ASSERT_EQ ( ast_empty.operation_to_string(), "Empty" );
    ASSERT_EQ ( ast_int.operation_to_string(), "200" );
    ASSERT_EQ ( ast_float.operation_to_string(), "2.37" );
    ASSERT_EQ ( ast_add.operation_to_string(), "Addition" );
}

TEST ( AST, is_num )
{
    pfme::AST ast_empty ( pfme::AST_TYPE::EMPTY, "empty" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_add ( pfme::AST_TYPE::ADDITION, "+" );

    ASSERT_EQ ( ast_empty.is_num(), true );
    ASSERT_EQ ( ast_int.is_num(), true );
    ASSERT_EQ ( ast_float.is_num(), true );
    ASSERT_EQ ( ast_add.is_num(), false );
}

TEST ( AST, plus_operator )
{
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "5" );

    ASSERT_EQ ( std::get<long long int> ( ( ast_int + ast_int ).m_number ), 5 + 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_int + ast_float ).m_number ), 5 + 2.37 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float + ast_int ).m_number ), 2.37 + 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float + ast_float ).m_number ), 2.37 + 2.37 );
}

TEST ( AST, minus_operator )
{
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "5" );

    ASSERT_EQ ( std::get<long long int> ( ( ast_int - ast_int ).m_number ), 5 - 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_int - ast_float ).m_number ), 5 - 2.37 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float - ast_int ).m_number ), 2.37 - 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float - ast_float ).m_number ), 2.37 - 2.37 );
}
TEST ( AST, multiplication_operator )
{
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "5" );

    ASSERT_EQ ( std::get<long long int> ( ( ast_int * ast_int ).m_number ), 5 * 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_int * ast_float ).m_number ), 5 * 2.37 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float * ast_int ).m_number ), 2.37 * 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float * ast_float ).m_number ), 2.37 * 2.37 );
}
TEST ( AST, division_operator )
{
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "5" );
    pfme::AST ast_three ( pfme::AST_TYPE::INTEGER, "3" );
    pfme::AST ast_zero_f ( pfme::AST_TYPE::FLOAT, "0.0" );
    pfme::AST ast_zero_i ( pfme::AST_TYPE::FLOAT, "0.0" );


    ASSERT_EQ ( std::get<long long int> ( ( ast_int / ast_int ).m_number ), 5 / 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_int / ast_three ).m_number ), static_cast<long double> ( 5 ) / 3 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_int / ast_float ).m_number ), 5 / 2.37 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float / ast_int ).m_number ), 2.37 / 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float / ast_float ).m_number ), 2.37 / 2.37 );

    ASSERT_ANY_THROW ( ast_int / ast_zero_i );
    ASSERT_ANY_THROW ( ast_int / ast_zero_f );
    ASSERT_ANY_THROW ( ast_float / ast_zero_i );
    ASSERT_ANY_THROW ( ast_float / ast_zero_f );
}
TEST ( AST, exponent_operator )
{
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.37" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "5" );
    pfme::AST ast_zero_f ( pfme::AST_TYPE::FLOAT, "0.0" );
    pfme::AST ast_zero_i ( pfme::AST_TYPE::FLOAT, "0.0" );


    ASSERT_EQ ( std::get<long long int> ( ( ast_int ^ ast_int ).m_number ), 5 * 5 * 5 * 5 * 5 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_int ^ ast_float ).m_number ), std::powl ( 5l, 2.37l ) );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float ^ ast_int ).m_number ), std::powl ( 2.37l, 5l ) );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ( ast_float ^ ast_float ).m_number ), std::powl ( 2.37l, 2.37l ) );

    ASSERT_EQ ( std::get<long long int> ( ( ast_int ^ ast_zero_i ).m_number ), 1 );
    ASSERT_EQ ( std::get<long long int> ( ( ast_int ^ ast_zero_f ).m_number ), 1 );
    ASSERT_EQ ( std::get<long long int> ( ( ast_float ^ ast_zero_i ).m_number ), 1 );
    ASSERT_EQ ( std::get<long long int> ( ( ast_float ^ ast_zero_f ).m_number ), 1 );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
