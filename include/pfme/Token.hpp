#pragma once
#include <iostream>
#include <string>
namespace pfme
{
/**
 * Enum for the Token types.
 */
enum class TOKEN_TYPE : char
{
    TOKEN_L_PAREN        = '(', /**< Left parenthesis, represented by ( */
    TOKEN_R_PAREN        = ')', /**< Right parenthesis, represented by ) */
    TOKEN_MULTIPLICATION = '*', /**< Multiplication, represented by * */
    TOKEN_DIVISION       = '/', /**< Division, represented by / */
    TOKEN_ADDITION       = '+', /**< Addition, represented by + */
    TOKEN_SUBTRACTION    = '-', /**< Subtraction, represented by - */
    TOKEN_EXPONENTIATION = '^', /**< Exponentiation, represented by ^ */
    TOKEN_INTEGER        = '0', /**< Integer, represented by 0 */
    TOKEN_FLOAT          = '.', /**< Float, represented by . */
    TOKEN_EOF            = '#', /**< End of File, represented by # */
    TOKEN_UNKNOWN        = '?', /**< Unknown, represented by ? */
};

/**
 * Get a string for each TOKEN_TYPE.
 * @param type is the TOKEN_TYPE for which you want the string
 * @return Is a string for that TOKEN_TYPE (e.g. for pfme::TOKEN_TYPE::TOKEN_FLOAT returns "TOKEN_FLOAT")
 */
auto token_type_to_string ( TOKEN_TYPE type ) -> std::string;

/**
 * The Token class.
 * A Token represents a symbol or a sequences of symbols of a specific type.
 */
class Token
{
public:
    Token()                           = default;
    Token ( const Token& )            = default;
    Token ( Token&& )                 = default;
    ~Token()                          = default;
    Token& operator= ( const Token& ) = default;
    Token& operator= ( Token&& )      = default;

    /**
	 * Initialises the type and the value of the Token.
	 * @param type is the type of the Token
	 * @param value is the value of the Token, generally a character representing it or a sequence of digits for numbers
	 */
    Token ( TOKEN_TYPE type, std::string value )
        : m_type ( type )
        , m_value ( std::move ( value ) )
    {
    }

    /**
	 * Initialises the type and the value of the Token.
	 * @param type is the type of the Token as a char
	 * @param value is the value of the Token, generally a character representing it or a sequence of digits for numbers
	 */
    Token ( char type, std::string value )
        : m_type ( static_cast<TOKEN_TYPE> ( type ) )
        , m_value ( std::move ( value ) )
    {
    }

    /**
	 * Getter for the type.
	 * @return The TOKEN_TYPE of the Token
	 */
    auto get_type() const -> TOKEN_TYPE { return m_type; }
    /**
	 * Getter for the value.
	 * @return The value of the Token
	 */
    auto get_value() const -> std::string { return m_value; }

private:
    TOKEN_TYPE  m_type { TOKEN_TYPE::TOKEN_UNKNOWN };
    std::string m_value {};
};
} // namespace pfme