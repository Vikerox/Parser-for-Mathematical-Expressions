#pragma once
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <pfme/AST.hpp>
#include <pfme/Lexer.hpp>
#include <pfme/Token.hpp>

namespace pfme
{
/**
 * @brief The Parser class.
 * 
 * Parses the input string with the Lexer and constructs a binary tree containing all the nodes.
 * The binary tree will be constructed with the lowest operations (E.g + or -) at the top, operations of the same importance will be read from left to right.
 * Generally the deeper in the tree an operation is, the more important it is.
 * 
 * The input 2 + 3 + 4 will result in:
 * 
 *       +
 *      / \
 *     2   +
 *        / \
 *       3   4
 * 
 * The input 4 + 3 * 2 will result in:
 * 
 *       +
 *      / \
 *     4   *
 *        / \
 *       3   2
 * 
 * The input (4 + 3) * 2 will result in:
 * 
 *         *
 *        / \
 *       +   2
 *      / \
 *     4   3
 */
class Parser
{
public:
    /**
     * @brief Initialises the Parser by giving it a Lexer.
     * Sets the internal Lexer, the Lexer is advanced and the current token is set to the return value,
     * @param lexer is the Lexer that the Parser will use, should contain a string to be parsed
     */
    explicit Parser ( std::unique_ptr<Lexer>&& lexer );
    Parser ( const Parser& )            = default;
    Parser ( Parser&& )                 = default;
    ~Parser()                           = default;
    Parser& operator= ( const Parser& ) = default;
    Parser& operator= ( Parser&& )      = default;

    /**
	 * @brief Parses the input string (from the Lexer).
	 * 
	 * E.g. 1+2 will result in the following tree structure, the pointer to the +-node will be the return value:
	 * 
	 *       +
	 *      / \
	 *     1   2
	 * 
	 * @return Is a shared pointer to the root of the constructed binary tree
	 */
    auto parse() -> std::shared_ptr<AST>;
    /**
     * @brief Helper function to print the binary tree.
     * 
     * E.g. the tree for 1 + 2 will look like this:
     * 
     *     └──+
     *        ├──1
     *        └──2
     * 
     * @param node is a pointer to the root node from which the tree is printed
     */
    static auto print_binary_tree ( const AST* node ) -> void;
    /**
     * Getter for the root.
     * @return A shared pointer to the root node
     */
    [[nodiscard]] auto get_root() const -> std::shared_ptr<AST> { return m_root; }

private:
    std::unique_ptr<Lexer> m_lexer             = nullptr;
    std::unique_ptr<Token> m_current_token     = nullptr;
    std::shared_ptr<AST>   m_root              = nullptr;
    int                    m_parenthesis_level = 0;
    bool                   m_negative_sign     = false;

    auto                      parse_expression() -> void;
    auto                      eat ( TOKEN_TYPE token ) -> void;
    auto                      add_operation ( const std::shared_ptr<AST>& operation ) -> void;
    static auto               print_binary_tree ( const std::string& prefix, const AST* node, bool isLeft ) -> void;
    [[nodiscard]] static auto find_bottom ( std::shared_ptr<AST> start ) -> std::shared_ptr<AST>;
    [[nodiscard]] static auto find_parent ( std::shared_ptr<AST> start, const std::shared_ptr<AST>& bottom )
        -> std::shared_ptr<AST>;
};
} // namespace pfme