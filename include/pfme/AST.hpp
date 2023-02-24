#pragma once
#include <memory>
#include <string>
#include <variant>

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
	 * Get the value of the node depending on the node type.
	 * @return A variant with long long int (for integer nodes) and long double (for float nodes), will always return 0 if the node is not a number @see is_num()
	 */
    auto value_for_type() const -> std::variant<long long int, long double>
    {
        if ( m_type == AST_TYPE::INTEGER ) { return m_integer; }
        else if ( m_type == AST_TYPE::FLOAT ) { return m_float; }
        return 0;
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
};
} // namespace pfme