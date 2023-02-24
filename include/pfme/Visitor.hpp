#pragma once
#include <any>
#include <functional>
#include <memory>
#include <pfme/Parser.hpp>
namespace pfme
{
/**
 * @brief The Visitor class.
 * Will visit each node in the binary tree that the Parser constructed starting from the deepest node or the first node with two children.
 */
class Visitor
{
public:
    using num_t = std::variant<long long int, long double>;
    /**
     * Initialises the Visitor with a parser and will use parse().
     * @param parser is the parser that should be used
     */
    explicit Visitor ( std::unique_ptr<Parser>&& parser );

    /**
	 * @brief Will visit all nodes of the binary tree constructed by the parser.
	 * It will loop over the tree until the root node is a number, to do that it:
	 * - recursively looks for an operation node with two numbers as children
	 * - performs the operation
	 * - sets the operation node to a number node with the result:
	 * 
	 *         +           +
	 *        / \         / \
	 *       2   +  ---> 2   7
	 *          / \
	 *         3   4
	 * 
	 * @return Is a variant with the result of the calculation
	 */
    auto visit() -> num_t;
    /**
     * Helper function to print the tree.
     * @see Parser::print_binary_tree
     */
    auto print_tree() const -> void;
    /**
     * Setter for debug mode.
     * @param debug is the value of debug mode, true for on and false for off
     */
    auto set_debug_mode ( bool debug ) -> void { m_debug_mode = debug; }

private:
    std::unique_ptr<Parser>&& m_parser;
    bool                      m_debug_mode = false;

    static auto traverse_tree ( std::shared_ptr<AST> node ) -> std::shared_ptr<AST>;
    auto perform_operation ( const std::shared_ptr<AST>& operation, std::function<num_t ( num_t, num_t )>& bin_operator ) const
        -> void;
};
} // namespace pfme