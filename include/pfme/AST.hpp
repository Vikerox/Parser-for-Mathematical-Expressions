#pragma once
#include <cmath>
#include <iostream>
#include <memory>
#include <pfme/Fraction.hpp>
#include <stdexcept>
#include <string>
#include <variant>


/**
 * @brief The namespace for all the functions, enums, and classes.
 * 
 * pfme as a shorthand for <b>P</b>arser <b>f</b>or <b>M</b>athematical <b>E</b>xpressions, the name of the Project.
 */
namespace pfme
{
using LD  = long double;
using LLI = long long int;
/**
 * Enum for the types of AST nodes.
 */
enum class AST_TYPE : int
{
    MULTIPLICATION, /**< Multiplication */
    DIVISION,       /**< Division */
    ADDITION,       /**< Addition */
    SUBTRACTION,    /**< Subtraction */
    EXPONENTIATION, /**< Exponentiation */
    INTEGER,        /**< Integer */
    FLOAT,          /**< Float */
    EMPTY,          /**< Empty */
    FRACTION,
};

/**
 * @brief A struct for the AST nodes.
 * There are two general types: 
 * - Operation nodes that have numbers or other operation nodes as children
 * - Number nodes that do not have children
 */
struct AST
{
    using num_t                  = std::variant<LLI, LD, Fraction>;
    AST_TYPE             m_type  = AST_TYPE::EMPTY; /**< The type of the node, set to empty. */
    std::string          m_value = "0"; /**< The Value of the node, generally either the number as a string or the operation. */
    num_t                m_number          = 0LL;
    int                  m_operation_level = 0; /**< The operation level, used to determine how the nodes are ordered. */
    std::shared_ptr<AST> lhand =
        nullptr; /**< The left child node, will typically never be empty unless the node is a number. */
    std::shared_ptr<AST> rhand = nullptr; /**< The right child node. */


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
        if ( m_type == AST_TYPE::INTEGER ) { m_number = std::stoll ( m_value ); }
        else if ( m_type == AST_TYPE::FLOAT ) { m_number = std::stold ( m_value ); }
    }
    /**
     * Helper constructor, sets the node as type integer.
     * @param number is the integer number the node will be set to
     */
    explicit AST ( LLI number )
        : m_type ( AST_TYPE::INTEGER )
        , m_value ( std::to_string ( number ) )
        , m_number ( number )
    {
    }
    /**
     * Helper constructor, sets the node as type float without losing precision.
     * @param number is the float number the node will be set to
     */
    explicit AST ( LD number )
        : m_type ( AST_TYPE::FLOAT )
        , m_value ( std::to_string ( number ) )
        , m_number ( number )
    {
    }
    /**
     * Helper constructor, sets the node as type fraction.
     * @param number is the fraction number the node will be set to
     */
    explicit AST ( Fraction number )
        : m_type ( AST_TYPE::FRACTION )
        , m_value ( number.to_string() )
        , m_number ( number )
    {
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
        case AST_TYPE::FRACTION: [[fallthrough]];
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
        return ( m_type == AST_TYPE::INTEGER || m_type == AST_TYPE::FLOAT || m_type == AST_TYPE::EMPTY ||
                 m_type == AST_TYPE::FRACTION );
    }

    friend auto operator+ ( const AST& lhs, const AST& rhs ) -> AST;
    friend auto operator- ( const AST& lhs, const AST& rhs ) -> AST;
    friend auto operator* ( const AST& lhs, const AST& rhs ) -> AST;
    friend auto operator/ ( const AST& lhs, const AST& rhs ) -> AST;
    friend auto operator^ ( const AST& lhs, const AST& rhs ) -> AST;
    friend auto operator<< ( std::ostream& stream, const AST& obj ) -> std::ostream&;
};
} // namespace pfme