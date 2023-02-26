#include <pfme/Parser.hpp>
namespace pfme
{
//NOLINTBEGIN(misc-no-recursion)
auto Parser::print_binary_tree ( const std::string& prefix, const AST* node, bool isLeft ) -> void
{
    if ( node != nullptr )
    {
        std::cout << prefix;

#pragma warning( suppress : 4566 )
        std::cout << ( isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->m_value << '\n';

        // enter the next tree level - left and right branch
#pragma warning( suppress : 4566 )
        print_binary_tree ( prefix + ( isLeft ? "│   " : "    " ), node->lhand.get(), true );
#pragma warning( suppress : 4566 )
        print_binary_tree ( prefix + ( isLeft ? "│   " : "    " ), node->rhand.get(), false );
    }
} //NOLINTEND(misc-no-recursion)

auto Parser::print_binary_tree ( const AST* node ) -> void
{
    print_binary_tree ( "", node, false );
    std::cout << '\n';
}

Parser::Parser ( std::unique_ptr<Lexer>&& lexer )
    : m_lexer ( std::move ( lexer ) )
    , m_current_token ( m_lexer->get_next_token() )
{
}

auto Parser::eat ( TOKEN_TYPE token ) -> void
{
    if ( this->m_current_token->get_type() == token )
    {
        this->m_current_token  = this->m_lexer->get_next_token();
        return;
    }

    throw std::runtime_error (
        m_lexer->error_string ( std::format ( "Unexpected token: '{}'\nIs {}\tShould be {}\n",
                                              this->m_current_token->get_value(),
                                              pfme::token_type_to_string ( this->m_current_token->get_type() ),
                                              token_type_to_string ( token ) ),
                                -1 ) );
}

auto Parser::parse() -> std::shared_ptr<AST>
{
    while ( this->m_current_token->get_type() != TOKEN_TYPE::TOKEN_EOF )
    {
        switch ( this->m_current_token->get_type() )
        {
        case TOKEN_TYPE::TOKEN_L_PAREN:
            ++this->m_parenthesis_level;
            eat ( TOKEN_TYPE::TOKEN_L_PAREN );
            break;
        case TOKEN_TYPE::TOKEN_SUBTRACTION: m_negative_sign = true;
        case TOKEN_TYPE::TOKEN_ADDITION: eat ( this->m_current_token->get_type() );
        case TOKEN_TYPE::TOKEN_INTEGER: [[fallthrough]];
        case TOKEN_TYPE::TOKEN_FLOAT: parse_expression(); break;
        default:
            throw std::runtime_error (
                m_lexer->error_string ( std::format ( "Invalid Token '{}' in expression\nShould be {}, {} or {}",
                                                      pfme::token_type_to_string ( this->m_current_token->get_type() ),
                                                      token_type_to_string ( TOKEN_TYPE::TOKEN_L_PAREN ),
                                                      token_type_to_string ( TOKEN_TYPE::TOKEN_INTEGER ),
                                                      token_type_to_string ( TOKEN_TYPE::TOKEN_FLOAT ) ),
                                        -1 ) );
        }
    }
    auto bot = find_bottom ( this->m_root );
    if ( bot != nullptr ) { bot->rhand = std::make_shared<AST> ( AST_TYPE::INTEGER, "0" ); }

    return this->m_root;
}

auto Parser::parse_expression() -> void
{
    std::shared_ptr<AST> number (
        new AST ( TOKEN_TYPE::TOKEN_INTEGER == m_current_token->get_type() ? AST_TYPE::INTEGER : AST_TYPE::FLOAT,
                  m_current_token->get_value().insert ( 0, ( m_negative_sign ? "-" : "" ) ) ) );

    eat ( m_current_token->get_type() );
    // special case ')' -> eat token (get next token)
    while ( m_current_token->get_type() == TOKEN_TYPE::TOKEN_R_PAREN )
    {
        --m_parenthesis_level;
        eat ( m_current_token->get_type() );
    }
    const std::shared_ptr<AST> operation ( new AST ( std::move ( number ) ) );
    operation->m_operation_level += m_parenthesis_level * 3;
    switch ( m_current_token->get_type() )
    {
    case TOKEN_TYPE::TOKEN_MULTIPLICATION:
        operation->m_type  = AST_TYPE::MULTIPLICATION;
        operation->m_value = "*";
        ++operation->m_operation_level;
        break;
    case TOKEN_TYPE::TOKEN_DIVISION:
        operation->m_type  = AST_TYPE::DIVISION;
        operation->m_value = "/";
        ++operation->m_operation_level;
        break;
    case TOKEN_TYPE::TOKEN_ADDITION:
        operation->m_type  = AST_TYPE::ADDITION;
        operation->m_value = "+";
        break;
    case TOKEN_TYPE::TOKEN_SUBTRACTION:
        operation->m_type  = AST_TYPE::SUBTRACTION;
        operation->m_value = "-";
        break;
    case TOKEN_TYPE::TOKEN_EXPONENTIATION:
        operation->m_type  = AST_TYPE::EXPONENTIATION;
        operation->m_value = "^";
        operation->m_operation_level += 2;
        break;
    case TOKEN_TYPE::TOKEN_EOF:
        // if m_root is empty (happens if the input is only one number) set m_root to the one know operation
        if ( this->m_root == nullptr ) { this->m_root = std::move ( operation->lhand ); }
        // otherwise when the end is reached, add the last number to the (theoretically) last empty slot
        else { find_bottom ( this->m_root )->rhand = std::move ( operation->lhand ); }
        return;
    default:
        throw std::runtime_error (
            m_lexer->error_string ( std::format ( "Invalid Token '{}' in expression\nShould be {}, {}, {}, {} or {}",
                                                  pfme::token_type_to_string ( m_current_token->get_type() ),
                                                  token_type_to_string ( TOKEN_TYPE::TOKEN_MULTIPLICATION ),
                                                  token_type_to_string ( TOKEN_TYPE::TOKEN_DIVISION ),
                                                  token_type_to_string ( TOKEN_TYPE::TOKEN_ADDITION ),
                                                  token_type_to_string ( TOKEN_TYPE::TOKEN_SUBTRACTION ),
                                                  token_type_to_string ( TOKEN_TYPE::TOKEN_EXPONENTIATION ) ),
                                    -1 ) );
    }
    add_operation ( operation );
    eat ( m_current_token->get_type() );
}

auto Parser::add_operation ( const std::shared_ptr<AST>& operation ) -> void
{
    if ( this->m_root == nullptr )
    {
        this->m_root = operation;
        return;
    }
    auto bottom = find_bottom ( this->m_root );
    if ( bottom->m_operation_level <= operation->m_operation_level ) { bottom->rhand = operation; }
    else
    {
        // case 5 * 1 + [...]
        //               +
        //   *     +    / \
		//  /  +  / =  *  [...]
        // 5     1    / \
		//           5   1
        bottom->rhand = std::move ( operation->lhand );
        if ( !bottom->lhand->is_num() ) { std::swap ( bottom->rhand, bottom->lhand ); }
        auto node = bottom;

        while ( node != nullptr )
        {
            if ( node->m_operation_level <= operation->m_operation_level ) { break; }
            node = find_parent ( this->m_root, node );
        }
        if ( node == nullptr )
        {
            auto head           = this->m_root;
            this->m_root        = operation;
            this->m_root->lhand = head;
        }
        else
        {
            operation->lhand = node->rhand;
            node->rhand      = operation;
        }
    }
}

// recursively search through the tree for the first free position (left before right)
//NOLINTBEGIN(misc-no-recursion)
auto Parser::find_bottom ( std::shared_ptr<AST> start ) -> std::shared_ptr<AST>
{
    if ( start->m_type == AST_TYPE::INTEGER || start->m_type == AST_TYPE::FLOAT ) { return nullptr; }

    if ( start->rhand == nullptr ) { return start; }

    if ( start->lhand->m_type == AST_TYPE::INTEGER || start->lhand->m_type == AST_TYPE::FLOAT )
    {
        if ( start->rhand->m_type == AST_TYPE::INTEGER || start->rhand->m_type == AST_TYPE::FLOAT ) { return nullptr; }
    }
    else if ( !start->rhand->is_num() )
    {
        auto position = find_bottom ( start->lhand );
        if ( position != nullptr ) { return position; }
    }
    return find_bottom ( start->rhand );
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto Parser::find_parent ( std::shared_ptr<AST> start, const std::shared_ptr<AST>& bottom ) -> std::shared_ptr<AST>
{
    if ( start->lhand == bottom || start->rhand == bottom ) { return start; }
    if ( start == bottom ) { return nullptr; }

    if ( start->lhand->m_type == AST_TYPE::INTEGER || start->lhand->m_type == AST_TYPE::FLOAT )
    {
        if ( start->rhand->m_type == AST_TYPE::INTEGER || start->rhand->m_type == AST_TYPE::FLOAT ) { return nullptr; }
        return find_parent ( start->rhand, bottom );
    }
    auto position = find_parent ( start->lhand, bottom );
    if ( position != nullptr ) { return position; }
    return find_parent ( start->rhand, bottom );

} //NOLINTEND(misc-no-recursion)
} // namespace pfme