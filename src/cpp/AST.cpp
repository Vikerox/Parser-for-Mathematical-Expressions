#include <pfme/AST.hpp>
namespace pfme
{
auto operator+ ( const AST& lhs, const AST& rhs ) -> AST
{
    std::string    res {};
    const AST_TYPE res_type =
        lhs.m_type == AST_TYPE::FLOAT || rhs.m_type == AST_TYPE::FLOAT ? AST_TYPE::FLOAT : AST_TYPE::INTEGER;
    std::visit ( [&] ( auto add1, auto add2 ) { res = std::to_string ( add1 + add2 ); }, lhs.m_number, rhs.m_number );
    return { res_type, res };
}

auto operator- ( const AST& lhs, const AST& rhs ) -> AST
{
    std::string    res {};
    const AST_TYPE res_type =
        lhs.m_type == AST_TYPE::FLOAT || rhs.m_type == AST_TYPE::FLOAT ? AST_TYPE::FLOAT : AST_TYPE::INTEGER;
    std::visit (
        [&] ( auto minuend, auto subtrahend ) { res = std::to_string ( minuend - subtrahend ); }, lhs.m_number, rhs.m_number );
    return { res_type, res };
}

auto operator* ( const AST& lhs, const AST& rhs ) -> AST
{
    std::string    res {};
    const AST_TYPE res_type =
        lhs.m_type == AST_TYPE::FLOAT || rhs.m_type == AST_TYPE::FLOAT ? AST_TYPE::FLOAT : AST_TYPE::INTEGER;
    std::visit (
        [&] ( auto factor1, auto factor2 ) { res = std::to_string ( factor1 * factor2 ); }, lhs.m_number, rhs.m_number );
    return { res_type, res };
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
        return { AST_TYPE::INTEGER,
                 std::to_string ( std::get<long long int> ( lhs.m_number ) / std::get<long long int> ( rhs.m_number ) ) };
    }
    std::string    res {};
    const AST_TYPE res_type = AST_TYPE::FLOAT;
    std::visit ( [&] ( auto div1, auto div2 ) { res = std::to_string ( static_cast<long double> ( div1 ) / div2 ); },
                 lhs.m_number,
                 rhs.m_number );
    return { res_type, res };
}

auto operator^ ( const AST& lhs, const AST& rhs ) -> AST
{
    if ( ( rhs.m_type == AST_TYPE::INTEGER && std::get<long long int> ( rhs.m_number ) == 0 ) ||
         ( rhs.m_type == AST_TYPE::FLOAT && std::get<long double> ( rhs.m_number ) == 0 ) )
    {
        return { AST_TYPE::INTEGER, std::to_string ( 1 ) };
    }
    if ( lhs.m_type == AST_TYPE::INTEGER &&
         rhs.m_type == AST_TYPE::INTEGER ) // exponentiation while preserving the integer type
    {
        long long int res = 1;
        for ( long long int i = 0; i < std::get<long long int> ( rhs.m_number ); ++i )
        {
            res *= std::get<long long int> ( lhs.m_number );
        }
        return { AST_TYPE::INTEGER, std::to_string ( res ) };
    }
    std::string    res {};
    const AST_TYPE res_type = AST_TYPE::FLOAT; // result will always be float
    std::visit (
        [&] ( auto base, auto exponent ) {
            res = std::to_string ( std::powl ( static_cast<long double> ( base ), static_cast<long double> ( exponent ) ) );
        },
        lhs.m_number,
        rhs.m_number );
    return { res_type, res };
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