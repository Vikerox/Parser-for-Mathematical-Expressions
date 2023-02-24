#include "pfme/Lexer.hpp"
namespace pfme
{
Lexer::Lexer ( std::string_view data )
    : m_contents ( data )
    , m_current_char ( m_contents[m_index] )
{
}

auto Lexer::get_next_token() -> std::unique_ptr<Token>
{
    while ( m_current_char != '\0' && m_index < m_contents.length() )
    {
        if ( m_current_char == ' ' || m_current_char == '\n' || m_current_char == '\t' ) { skip_whitespace(); }

        // check if it is a valid character and throw if not
        if ( m_current_char < 0 ) { throw std::runtime_error ( error_string ( "character too large" ) ); }
        if ( isdigit ( m_current_char ) != 0 || m_current_char == '.' ) { return collect_number(); }
        auto token = std::make_unique<Token> ( m_current_char, get_current_char_as_string() );
        advance();
        return token;
    }

    return std::make_unique<Token> ( TOKEN_TYPE::TOKEN_EOF, "\0" );
}


auto Lexer::advance() -> void
{
    if ( m_current_char != '\0' && m_index < m_contents.length() )
    {
        ++m_index;
        m_current_char = m_contents[m_index];
    }
}

auto Lexer::skip_whitespace() -> void
{
    while ( m_current_char == ' ' || m_current_char == '\n' || m_current_char == '\t' ) { advance(); }
}

auto Lexer::collect_number() -> std::unique_ptr<Token>
{
    std::string value {};
    bool        point = false;
    while ( isdigit ( m_current_char ) != 0 || ( m_current_char == '.' && !point ) )
    {
        if ( m_current_char == '.' ) { point = true; }
        auto collector = get_current_char_as_string();
        value += collector;
        advance();
    }
    if ( m_current_char == '.' && point )
    {
        throw std::runtime_error ( error_string ( "two points in one number are not allowed" ).c_str() );
    }

    if ( point ) { return std::make_unique<Token> ( TOKEN_TYPE::TOKEN_FLOAT, value ); }
    return std::make_unique<Token> ( TOKEN_TYPE::TOKEN_INTEGER, value );
}

auto Lexer::get_current_char_as_string() const -> std::string { return { m_current_char }; }

auto Lexer::error_string ( std::string_view msg, int index_modifier ) -> std::string
{
    return std::format ( "{}\n"
                         "First found here: \n"
                         "\t {}\n"
                         "\t {}",
                         msg,
                         m_contents,
                         std::string ( m_index + index_modifier, ' ' ) + "^\n" );
}
} // namespace pfme