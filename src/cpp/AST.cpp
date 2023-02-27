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
    if ( ( rhs.m_type == AST_TYPE::INTEGER && std::get<LLI> ( rhs.m_number ) == 0 ) ||
         ( rhs.m_type == AST_TYPE::FLOAT && std::get<LD> ( rhs.m_number ) == 0 ) )
    {
        throw std::runtime_error ( "Division by Zero" );
    }
    if ( lhs.m_type == AST_TYPE::INTEGER &&
         rhs.m_type == AST_TYPE::INTEGER ) // preserve the type if there is a "clean" integer division
    {
        const Fraction temp { std::get<LLI> ( lhs.m_number ), std::get<LLI> ( rhs.m_number ) };
        if ( temp.is_whole() ) { return AST { static_cast<LLI> ( temp ) }; }
        return AST { temp };
    }

    AST node {};
    std::visit ( [&] ( auto div1, auto div2 ) { node = AST { div1 / div2 }; }, lhs.m_number, rhs.m_number );
    return AST { node };
}

auto operator^ ( const AST& lhs, const AST& rhs ) -> AST
{
    if ( ( rhs.m_type == AST_TYPE::INTEGER && std::get<LLI> ( rhs.m_number ) == 0 ) ||
         ( rhs.m_type == AST_TYPE::FLOAT && std::get<LD> ( rhs.m_number ) == 0 ) )
    {
        return AST { 1LL };
    }
    if ( lhs.m_type == AST_TYPE::INTEGER &&
         rhs.m_type == AST_TYPE::INTEGER ) // exponentiation while preserving the integer type
    {
        LLI res = 1;
        for ( LLI i = 0; i < std::get<LLI> ( rhs.m_number ); ++i ) { res *= std::get<LLI> ( lhs.m_number ); }
        return AST { res };
    }

    LD res {};
    std::visit (
        [&] ( auto base, auto exponent ) { res = std::powl ( static_cast<LD> ( base ), static_cast<LD> ( exponent ) ); },
        lhs.m_number,
        rhs.m_number );
    return AST { res };
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