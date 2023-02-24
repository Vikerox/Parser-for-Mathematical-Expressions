#include <gtest/gtest.h>
#include <pfme/AST.hpp>

TEST ( AST, Constructor )
{
    pfme::AST ast_std;
    pfme::AST ast_t_and_v_i ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_t_and_v_f ( pfme::AST_TYPE::FLOAT, "3.6" );
    pfme::AST ast_node ( std::make_shared<pfme::AST> ( ast_std ) );

    ASSERT_EQ ( ast_std.m_type, pfme::AST_TYPE::EMPTY );
    ASSERT_EQ ( ast_std.m_operation_level, 0 );
    ASSERT_EQ ( ast_std.m_value, "0" );

    ASSERT_EQ ( ast_t_and_v_i.m_type, pfme::AST_TYPE::INTEGER );
    ASSERT_EQ ( ast_t_and_v_i.m_value, "200" );
    ASSERT_EQ ( ast_t_and_v_i.m_integer, 200 );
    ASSERT_DOUBLE_EQ ( ast_t_and_v_i.m_float, 0 );

    ASSERT_EQ ( ast_t_and_v_f.m_type, pfme::AST_TYPE::FLOAT );
    ASSERT_EQ ( ast_t_and_v_f.m_value, "3.6" );
    ASSERT_EQ ( ast_t_and_v_f.m_integer, 0 );
    ASSERT_DOUBLE_EQ ( ast_t_and_v_f.m_float, 3.6 );

    ASSERT_EQ ( ast_node.m_type, pfme::AST_TYPE::EMPTY );
    ASSERT_EQ ( ast_node.rhand, nullptr );
    ASSERT_NE ( ast_node.lhand, nullptr );
}

TEST ( AST, value_for_type )
{
    pfme::AST ast_empty ( pfme::AST_TYPE::EMPTY, "empty" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.5" );

    ASSERT_EQ ( std::get<long long int> ( ast_empty.value_for_type() ), 0 );
    ASSERT_EQ ( std::get<long long int> ( ast_int.value_for_type() ), 200 );
    ASSERT_DOUBLE_EQ ( std::get<long double> ( ast_float.value_for_type() ), 2.5 );
}

TEST ( AST, operation_to_string )
{
    pfme::AST ast_empty ( pfme::AST_TYPE::EMPTY, "empty" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.5" );
    pfme::AST ast_add ( pfme::AST_TYPE::ADDITION, "+" );

    ASSERT_EQ ( ast_empty.operation_to_string(), "Empty" );
    ASSERT_EQ ( ast_int.operation_to_string(), "200" );
    ASSERT_EQ ( ast_float.operation_to_string(), "2.5" );
    ASSERT_EQ ( ast_add.operation_to_string(), "Addition" );
}

TEST ( AST, is_num )
{
    pfme::AST ast_empty ( pfme::AST_TYPE::EMPTY, "empty" );
    pfme::AST ast_int ( pfme::AST_TYPE::INTEGER, "200" );
    pfme::AST ast_float ( pfme::AST_TYPE::FLOAT, "2.5" );
    pfme::AST ast_add ( pfme::AST_TYPE::ADDITION, "+" );

    ASSERT_EQ ( ast_empty.is_num(), true );
    ASSERT_EQ ( ast_int.is_num(), true );
    ASSERT_EQ ( ast_float.is_num(), true );
    ASSERT_EQ ( ast_add.is_num(), false );
}

int main ( int argc, char** argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS();
}
