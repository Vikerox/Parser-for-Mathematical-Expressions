#pragma once
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>


/**
 * @brief The namespace for all the functions, enums, and classes.
 * 
 * pfme as a shorthand for <b>P</b>arser <b>f</b>or <b>M</b>athematical <b>E</b>xpressions, the name of the Project.
 */
namespace pfme
{
/**
 * Enum for the types of AST nodes.
 */
enum class AST_TYPE
{
    MULTIPLICATION, /**< Multiplication */
    DIVISION,       /**< Division */
    ADDITION,       /**< Addition */
    SUBTRACTION,    /**< Subtraction */
    EXPONENTIATION, /**< Exponentiation */
    INTEGER,        /**< Integer */
    FLOAT,          /**< Float */
    EMPTY,          /**< Empty */
};

/**
 * @brief A struct for the AST nodes.
 * There are two general types: 
 * - Operation nodes that have numbers or other operation nodes as children
 * - Number nodes that do not have children
 */
struct AST
{
    AST_TYPE             m_type  = AST_TYPE::EMPTY; /**< The type of the node, set to empty. */
    std::string          m_value = "0"; /**< The Value of the node, generally either the number as a string or the operation. */
    int                  m_operation_level = 0; /**< The operation level, used to determine how the nodes are ordered. */
    std::shared_ptr<AST> lhand =
        nullptr; /**< The left child node, will typically never be empty unless the node is a number. */
    std::shared_ptr<AST> rhand     = nullptr; /**< The right child node. */
    long long int        m_integer = 0;       /**< Stores the value as a long long int if the number is of type integer. */
    long double          m_float   = 0;       /**< Stores the value as a long double if the number is of type float. */


    AST()              = default;
    AST ( const AST& ) = default;
    AST ( AST&& )      = default;
    ~AST()             = default;

    AST& operator= ( const AST& ) = default;
    AST& operator= ( AST&& )      = default;

    /**
	 * Sets m_type and m_value, also sets the respective values for m_integer and m_float if the it fits the respective type.
	 * @param type is the type of the node
	 * @param value is the value of the node
	 */
    AST ( AST_TYPE type, const std::string& value )
        : m_type ( type )
        , m_value ( value )
    {
        if ( m_type == AST_TYPE::INTEGER ) { m_integer = std::stoll ( m_value ); }
        else if ( m_type == AST_TYPE::FLOAT ) { m_float = std::stold ( m_value ); }
    }
    /**
     * Sets the left hand child node (all other values will be set to their defaults).
     * @param left_hand will become the lhand of the node
     */
    explicit AST ( std::shared_ptr<AST>&& left_hand )
        : lhand ( std::move ( left_hand ) )
    {
    }

    /**
	 * Get a string corresponding to each type of node.
	 * @return The name of the node type or the value for numbers @see is_num()
	 */
    auto operation_to_string() const -> std::string
    {
        switch ( m_type )
        {
        case AST_TYPE::MULTIPLICATION: return "Multipication";
        case AST_TYPE::DIVISION: return "Division";
        case AST_TYPE::ADDITION: return "Addition";
        case AST_TYPE::SUBTRACTION: return "Subtraction";
        case AST_TYPE::EXPONENTIATION: return "Exponentiation";
        case AST_TYPE::INTEGER: [[fallthrough]];
        case AST_TYPE::FLOAT: return m_value;
        case AST_TYPE::EMPTY: return "Empty";
        default: return "";
        }
    }

    /**
	 * Tells you wether or not the node is a number.
	 * @return true for integer, floats or empty, false for all others
	 */
    auto is_num() const -> bool
    {
        return ( m_type == AST_TYPE::INTEGER || m_type == AST_TYPE::FLOAT || m_type == AST_TYPE::EMPTY );
    }

    friend auto operator+ ( AST lhs, const AST& rhs ) -> AST
    {
        if ( lhs.m_type == AST_TYPE::FLOAT && rhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( lhs.m_float + rhs.m_float ) };
        }
        else if ( lhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( lhs.m_float + rhs.m_integer ) };
        }
        else if ( rhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( lhs.m_integer + rhs.m_float ) };
        }
        return { AST_TYPE::INTEGER, std::to_string ( lhs.m_integer + rhs.m_integer ) };
    }

    friend auto operator- ( AST lhs, const AST& rhs ) -> AST
    {
        AST neg_rhs = rhs;
        neg_rhs.m_float *= -1;
        neg_rhs.m_integer *= -1;
        return lhs + neg_rhs;
    }

    friend auto operator* ( AST lhs, const AST& rhs ) -> AST
    {
        if ( lhs.m_type == AST_TYPE::FLOAT && rhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( lhs.m_float * rhs.m_float ) };
        }
        else if ( lhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( lhs.m_float * rhs.m_integer ) };
        }
        else if ( rhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( lhs.m_integer * rhs.m_float ) };
        }
        return { AST_TYPE::INTEGER, std::to_string ( lhs.m_integer * rhs.m_integer ) };
    }

    friend auto operator/ ( AST lhs, const AST& rhs ) -> AST
    {
        if ( ( rhs.m_type == AST_TYPE::INTEGER && rhs.m_integer == 0 ) ||
             ( rhs.m_type == AST_TYPE::FLOAT && rhs.m_float == 0 ) )
        {
            throw std::runtime_error ( "Division by Zero" );
        }
        if ( lhs.m_type == AST_TYPE::INTEGER && rhs.m_type == AST_TYPE::INTEGER && lhs.m_integer % rhs.m_integer == 0 )
        {
            return { AST_TYPE::INTEGER, std::to_string ( lhs.m_integer / rhs.m_integer ) };
        }
        else
        {
            AST inv_rhs = rhs;
            if ( inv_rhs.m_type == AST_TYPE::FLOAT ) { inv_rhs.m_float = 1 / inv_rhs.m_float; }
            else
            {
                inv_rhs.m_type  = AST_TYPE::FLOAT;
                inv_rhs.m_float = static_cast<long double> ( 1 ) / inv_rhs.m_integer;
            }
            return lhs * inv_rhs;
        }
    }

    friend auto operator^ ( AST lhs, const AST& rhs ) -> AST
    {
        if ( ( rhs.m_type == AST_TYPE::INTEGER && rhs.m_integer == 0 ) ||
             ( rhs.m_type == AST_TYPE::FLOAT && rhs.m_float == 0 ) )
        {
            return { AST_TYPE::INTEGER, std::to_string ( 1 ) };
        }
        if ( lhs.m_type == AST_TYPE::INTEGER && rhs.m_type == AST_TYPE::INTEGER )
        {
            long long int res = 1;
            for ( long long int i = 0; i < rhs.m_integer; ++i ) { res *= lhs.m_integer; }
            return { AST_TYPE::INTEGER, std::to_string ( res ) };
        }
        if ( lhs.m_type == AST_TYPE::FLOAT && rhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( std::powl ( lhs.m_float, rhs.m_float ) ) };
        }
        else if ( lhs.m_type == AST_TYPE::FLOAT )
        {
            return { AST_TYPE::FLOAT, std::to_string ( std::powl ( lhs.m_float, rhs.m_integer ) ) };
        }
        return { AST_TYPE::FLOAT, std::to_string ( std::powl ( lhs.m_integer, rhs.m_float ) ) };
    }

    friend auto operator<< ( std::ostream& os, const AST& obj ) -> std::ostream&
    {
        if ( obj.is_num() ) { os << obj.m_value; }
        else
        {
            os << "Operation: " << obj.lhand->operation_to_string() << ' ' << obj.operation_to_string() << ' '
               << obj.rhand->operation_to_string();
        }
        return os;
    }
};
} // namespace pfme