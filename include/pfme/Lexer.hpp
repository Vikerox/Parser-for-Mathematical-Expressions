#pragma once
#include <format>
#include <memory>
#include <pfme/Token.hpp>
#include <string>
#include <string_view>
#include <vector>
namespace pfme
{

/**
 * @brief The Lexer class.
 * It's job is iterating over the input string, find tokens, and collect numbers.
 */
class Lexer
{
public:
    /**
     * Initialises the Lexer with an input string (its content), the current char is set to the first character.
     * @param data is the input string for the Lexer
     */
    explicit Lexer ( std::string_view data );
    Lexer ( const Lexer& ) = default;
    Lexer ( Lexer&& )      = default;
    ~Lexer()               = default;

    Lexer& operator= ( const Lexer& ) = default;
    Lexer& operator= ( Lexer&& )      = default;

    Lexer ( std::string_view data, char point_sym, std::vector<char>&& seperators );
    /**
	 * Collects the next token.
	 * @return The found token as an unique_ptr
	 * @see Token
	 */
    auto get_next_token() -> std::unique_ptr<Token>;
    /**
     * Getter for the content.
     * @return The content of the Lexer as a string
     */
    auto get_content() const -> std::string { return m_contents; }
    /**
     * @brief Helper function for generating error messages.
     * The generated string once printed will look like this:
     * 
     *     <msg>
     *     First found here: 
     *          <the content>
     *          ^    <-- this should point to the position where the error occured, spaces are generated infront of it based on the internal index + index_modifier
     * 
     * @param msg will be the beginning of the string
     * @param index_modifier is set to zero, can be adjusted if the index is in the wrong place
     * @return A formatted error string, see above
     */
    auto error_string ( std::string_view msg, int index_modifier = 0 ) -> std::string;

private:
    unsigned int      m_index = 0;    /**< The index, the position of the Lexer in the string */
    std::string       m_contents;     /**< The content of the Lexer */
    char              m_current_char; /**< The current character, will be the same as m_contents[m_index] */
    char              m_point_symbol = '.';
    std::vector<char> m_seperators { '\'', ',', '_' };

    /**
	 * Advances the Lexer by one character, changes m_index and m_current_char.
	 */
    auto advance() -> void;

    /**
	 * Will advance the Lexer until m_current_char is not a space, a line end, or a tab.
	 */
    auto skip_whitespace() -> void;

    /**
	 * Collects a number and will also check wether the number has two points (it throws a runtime error if it does).
	 * @return Either a integer or float Token with the value set to the collected number
	 */
    auto collect_number() -> std::unique_ptr<Token>;

    /**
	 * Helper function.
	 * @return m_current_char as a string
	 */
    [[nodiscard]] auto get_current_char_as_string() const -> std::string;
};
} // namespace pfme