#include <pfme/Fraction.hpp>

namespace pfme
{

Fraction::Fraction ( LLI numerator )
    : m_numerator ( numerator )
{
}
Fraction::Fraction ( LLI numerator, LLI denominator )
    : m_numerator ( numerator )
    , m_denominator ( denominator )
{
    reduce();
}
auto Fraction::to_string() const -> std::string { return std::format ( "{} / {}", this->m_numerator, this->m_denominator ); }
auto Fraction::reduce() -> void
{
    if ( this->m_denominator == 0 ) { throw std::runtime_error ( "Division by zero" ); }
    if ( this->m_numerator == 0 )
    {
        this->m_denominator = 1;
        return;
    }
    auto gcd = std::gcd ( this->m_numerator, this->m_denominator );
    this->m_numerator /= gcd;
    this->m_denominator /= gcd;
}

auto Fraction::is_whole() const -> bool { return this->m_numerator % this->m_denominator == 0; }

auto Fraction::inverse() const -> Fraction { return Fraction ( this->m_denominator, this->m_numerator ); }

Fraction::operator LLI() const { return this->m_numerator / this->m_denominator; }

Fraction::operator LD() const { return static_cast<LD> ( this->m_numerator ) / static_cast<LD> ( this->m_denominator ); }
auto pfme::Fraction::operator-() const -> Fraction { return Fraction { -m_numerator, m_denominator }; }

// -----------------------------------------------------------------------------
//
//
//
//
// OPERATORS
//
//
//
//
// -----------------------------------------------------------------------------

auto operator<< ( std::ostream& stream, const Fraction& obj ) -> std::ostream&
{
    stream << obj.m_numerator << " / " << obj.m_denominator;
    return stream;
}

auto operator== ( const Fraction& lhs, const Fraction& rhs ) -> bool
{
    return ( lhs.m_numerator == rhs.m_numerator && lhs.m_denominator == rhs.m_denominator );
}

auto operator+ ( const LLI& lhs, const Fraction& rhs ) -> Fraction
{
    return Fraction { rhs.m_numerator + rhs.m_denominator * lhs, rhs.m_denominator };
}
auto operator- ( const LLI& lhs, const Fraction& rhs ) -> Fraction { return lhs + -rhs; }
auto operator* ( const LLI& lhs, const Fraction& rhs ) -> Fraction
{
    return Fraction { rhs.m_numerator * lhs, rhs.m_denominator };
}
auto operator/ ( const LLI& lhs, const Fraction& rhs ) -> Fraction { return lhs * rhs.inverse(); }
auto operator^ ( const LLI& lhs, const Fraction& rhs ) -> LD { return std::powl ( lhs, static_cast<LD> ( rhs ) ); }

auto operator+ ( const Fraction& lhs, const LLI& rhs ) -> Fraction { return rhs + lhs; }
auto operator- ( const Fraction& lhs, const LLI& rhs ) -> Fraction { return lhs + -rhs; }
auto operator* ( const Fraction& lhs, const LLI& rhs ) -> Fraction { return rhs * lhs; }
auto operator/ ( const Fraction& lhs, const LLI& rhs ) -> Fraction { return lhs.inverse() * rhs; }
auto operator^ ( const Fraction& lhs, const LLI& rhs ) -> Fraction
{
    return Fraction { static_cast<LLI> ( std::powl ( lhs.m_numerator, rhs ) ),
                      static_cast<LLI> ( std::powl ( lhs.m_denominator, rhs ) ) };
}

auto operator+ ( const Fraction& lhs, const Fraction& rhs ) -> Fraction
{
    return Fraction { lhs.m_numerator * rhs.m_denominator + rhs.m_numerator * lhs.m_denominator,
                      lhs.m_denominator * rhs.m_denominator };
}
auto operator- ( const Fraction& lhs, const Fraction& rhs ) -> Fraction { return lhs + -rhs; }
auto operator* ( const Fraction& lhs, const Fraction& rhs ) -> Fraction
{
    return Fraction { lhs.m_numerator * rhs.m_numerator, lhs.m_denominator * rhs.m_denominator };
}
auto operator/ ( const Fraction& lhs, const Fraction& rhs ) -> Fraction { return lhs * rhs.inverse(); }
auto operator^ ( const Fraction& lhs, const Fraction& rhs ) -> LD
{
    return std::powl ( static_cast<LD> ( lhs ), static_cast<LD> ( rhs ) );
}

auto operator+ ( const LD& lhs, const Fraction& rhs ) -> LD { return lhs + static_cast<LD> ( rhs ); }
auto operator- ( const LD& lhs, const Fraction& rhs ) -> LD { return lhs - static_cast<LD> ( rhs ); }
auto operator* ( const LD& lhs, const Fraction& rhs ) -> LD { return lhs * static_cast<LD> ( rhs ); }
auto operator/ ( const LD& lhs, const Fraction& rhs ) -> LD { return lhs / static_cast<LD> ( rhs ); }
auto operator^ ( const LD& lhs, const Fraction& rhs ) -> LD { return std::powl ( lhs, static_cast<LD> ( rhs ) ); }

auto operator+ ( const Fraction& lhs, const LD& rhs ) -> LD { return static_cast<LD> ( lhs ) + rhs; }
auto operator- ( const Fraction& lhs, const LD& rhs ) -> LD { return static_cast<LD> ( lhs ) - rhs; }
auto operator* ( const Fraction& lhs, const LD& rhs ) -> LD { return static_cast<LD> ( lhs ) * rhs; }
auto operator/ ( const Fraction& lhs, const LD& rhs ) -> LD { return static_cast<LD> ( lhs ) / rhs; }
auto operator^ ( const Fraction& lhs, const LD& rhs ) -> LD { return std::powl ( static_cast<LD> ( lhs ), rhs ); }

} // namespace pfme