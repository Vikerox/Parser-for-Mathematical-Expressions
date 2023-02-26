#include <pfme/Token.hpp>
namespace pfme
{

auto token_type_to_string ( TOKEN_TYPE type ) -> std::string
{
    std::string ret = "TOKEN_UNKNOWN";
    switch ( type )
    {
    case TOKEN_TYPE::TOKEN_L_PAREN: ret = "TOKEN_L_PAREN"; break;
    case TOKEN_TYPE::TOKEN_R_PAREN: ret = "TOKEN_R_PAREN"; break;
    case TOKEN_TYPE::TOKEN_MULTIPLICATION: ret = "TOKEN_MULTIPLICATION"; break;
    case TOKEN_TYPE::TOKEN_DIVISION: ret = "TOKEN_DIVISION"; break;
    case TOKEN_TYPE::TOKEN_ADDITION: ret = "TOKEN_ADDITION"; break;
    case TOKEN_TYPE::TOKEN_SUBTRACTION: ret = "TOKEN_SUBTRACTION"; break;
    case TOKEN_TYPE::TOKEN_EXPONENTIATION: ret = "TOKEN_EXPONENTIATION"; break;
    case TOKEN_TYPE::TOKEN_INTEGER: ret = "TOKEN_INTEGER"; break;
    case TOKEN_TYPE::TOKEN_FLOAT: ret = "TOKEN_FLOAT"; break;
    case TOKEN_TYPE::TOKEN_EOF: ret = "TOKEN_EOF"; break;
    case TOKEN_TYPE::TOKEN_UNKNOWN: break;
    default: break;
    }
    return ret;
}
} // namespace pfme