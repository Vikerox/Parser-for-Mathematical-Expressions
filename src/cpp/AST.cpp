#include <pfme/AST.hpp>
namespace pfme
{
auto operator+ ( const AST& lhs, const AST& rhs ) -> AST
{
    AST node;
    std::visit ( [&] ( auto add1, auto add2 ) { node = AST { add1 + add2 }; }, lhs.m_number, rhs.m_number );
    return node;
}

auto operator- ( const AST& lhs, const AST& rhs ) -> AST
{
    AST node;
    std::visit ( [&] ( auto minuend, auto subtrahend ) { node = AST { minuend - subtrahend }; }, lhs.m_number, rhs.m_number );
    return node;
}

auto operator* ( const AST& lhs, const AST& rhs ) -> AST
{
    AST node;
    std::visit ( [&] ( auto factor1, auto factor2 ) { node = AST { factor1 * factor2 }; }, lhs.m_number, rhs.m_number );
    return node;
}

auto operator/ ( const AST& lhs, const AST& rhs ) -> AST
{
    if ( ( rhs.m_type == AST_TYPE::INTEGER && std::get<long long int> ( rhs.m_number ) == 0 ) ||
         ( rhs.m_type == AST_TYPE::FLOAT && std::get<long double> ( rhs.m_number ) == 0 ) )
    {
        throw std::runtime_error ( "Division by Zero" );
    }
    if ( lhs.m_type == AST_TYPE::INTEGER && rhs.m_type == AST_TYPE::INTEGER &&
         std::get<long long int> ( lhs.m_number ) % std::get<long long int> ( rhs.m_number ) ==
             0 ) // preserve the type if there is a "clean" integer division
    {
        return { std::get<long long int> ( lhs.m_number ) / std::get<long long int> ( rhs.m_number ) };
    }
    long double res {};
    std::visit ( [&] ( auto div1, auto div2 ) { res = static_cast<long double> ( div1 ) / static_cast<long double> ( div2 ); },
                 lhs.m_number,
                 rhs.m_number );
    return { res };
}

auto operator^ ( const AST& lhs, const AST& rhs ) -> AST
{
    if ( ( rhs.m_type == AST_TYPE::INTEGER && std::get<long long int> ( rhs.m_number ) == 0 ) ||
         ( rhs.m_type == AST_TYPE::FLOAT && std::get<long double> ( rhs.m_number ) == 0 ) )
    {
        return { 1ll };
    }
    if ( lhs.m_type == AST_TYPE::INTEGER &&
         rhs.m_type == AST_TYPE::INTEGER ) // exponentiation while preserving the integer type
    {
        long long int res = 1;
        for ( long long int i = 0; i < std::get<long long int> ( rhs.m_number ); ++i )
        {
            res *= std::get<long long int> ( lhs.m_number );
        }
        return { res };
    }

    long double res {};
    std::visit (
        [&] ( auto base, auto exponent ) {
            res = std::powl ( static_cast<long double> ( base ), static_cast<long double> ( exponent ) );
        },
        lhs.m_number,
        rhs.m_number );
    return { res };
}

auto operator<< ( std::ostream& stream, const AST& obj ) -> std::ostream&
{
    if ( obj.is_num() ) { stream << obj.m_value; }
    else
    {
        stream << "Operation: " << obj.lhand->operation_to_string() << ' ' << obj.operation_to_string() << ' '
               << obj.rhand->operation_to_string();
    }
    return stream;
}
} // namespace pfme