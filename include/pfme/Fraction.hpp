#pragma once
#include <cmath>
#include <format>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace pfme
{
using LD  = long double;
using LLI = long long int;
class Fraction
{
public:
    Fraction()                              = default;
    Fraction ( const Fraction& )            = default;
    Fraction ( Fraction&& )                 = default;
    ~Fraction()                             = default;
    Fraction& operator= ( const Fraction& ) = default;
    Fraction& operator= ( Fraction&& )      = default;

    explicit Fraction ( LLI numerator );
    Fraction ( LLI numerator, LLI denominator );

    auto        to_string() const -> std::string;
    auto        is_whole() const -> bool;
    auto        inverse() const -> Fraction;
    explicit            operator LLI() const;
    explicit            operator LD() const;

    auto        operator-() const -> Fraction;
    friend auto operator<< ( std::ostream& stream, const Fraction& obj ) -> std::ostream&;
    friend auto operator== ( const Fraction& lhs, const Fraction& rhs ) -> bool;
    friend auto operator+ ( const LLI& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator- ( const LLI& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator* ( const LLI& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator/ ( const LLI& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator^ ( const LLI& lhs, const Fraction& rhs ) -> LD;
    friend auto operator+ ( const Fraction& lhs, const LLI& rhs ) -> Fraction;
    friend auto operator- ( const Fraction& lhs, const LLI& rhs ) -> Fraction;
    friend auto operator* ( const Fraction& lhs, const LLI& rhs ) -> Fraction;
    friend auto operator/ ( const Fraction& lhs, const LLI& rhs ) -> Fraction;
    friend auto operator^ ( const Fraction& lhs, const LLI& rhs ) -> Fraction;
    friend auto operator+ ( const Fraction& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator- ( const Fraction& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator* ( const Fraction& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator/ ( const Fraction& lhs, const Fraction& rhs ) -> Fraction;
    friend auto operator^ ( const Fraction& lhs, const Fraction& rhs ) -> LD;

    friend auto operator+ ( const LD& lhs, const Fraction& rhs ) -> LD;
    friend auto operator- ( const LD& lhs, const Fraction& rhs ) -> LD;
    friend auto operator* ( const LD& lhs, const Fraction& rhs ) -> LD;
    friend auto operator/ ( const LD& lhs, const Fraction& rhs ) -> LD;
    friend auto operator^ ( const LD& lhs, const Fraction& rhs ) -> LD;
    friend auto operator+ ( const Fraction& lhs, const LD& rhs ) -> LD;
    friend auto operator- ( const Fraction& lhs, const LD& rhs ) -> LD;
    friend auto operator* ( const Fraction& lhs, const LD& rhs ) -> LD;
    friend auto operator/ ( const Fraction& lhs, const LD& rhs ) -> LD;
    friend auto operator^ ( const Fraction& lhs, const LD& rhs ) -> LD;

private:
    LLI m_numerator { 1 };
    LLI m_denominator { 1 };

    auto reduce() -> void;
};
} // namespace pfme