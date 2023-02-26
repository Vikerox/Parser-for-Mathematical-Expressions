#include <pfme/Visitor.hpp>

namespace pfme
{
Visitor::Visitor ( std::unique_ptr<Parser>&& parser )
    : m_parser ( std::move ( parser ) )
{
    m_parser->parse();
}

auto Visitor::print_tree() const -> void { this->m_parser->print_binary_tree ( m_parser->get_root().get() ); }

auto Visitor::visit() -> std::string
{
    auto root = this->m_parser->get_root();
    while ( !root->is_num() )
    {
        const auto& operation = traverse_tree ( root );
        if ( m_debug_mode ) { std::cout << *operation; }
        switch ( operation->m_type )
        {
        case AST_TYPE::MULTIPLICATION: *operation = *operation->lhand * *operation->rhand; break;
        case AST_TYPE::DIVISION: *operation = *operation->lhand / *operation->rhand; break;
        case AST_TYPE::ADDITION: *operation = *operation->lhand + *operation->rhand; break;
        case AST_TYPE::SUBTRACTION: *operation = *operation->lhand - *operation->rhand; break;
        case AST_TYPE::EXPONENTIATION: *operation = *operation->lhand ^ *operation->rhand; break;
        default: break;
        }
        if ( m_debug_mode ) { std::cout << " = " << *operation << '\n'; }
    }
    return root->m_value;
}

//NOLINTBEGIN(misc-no-recursion)
auto Visitor::traverse_tree ( std::shared_ptr<AST>& node ) -> std::shared_ptr<AST>&
{
    if ( node->lhand->is_num() && node->rhand->is_num() ) { return node; }
    if ( node->lhand->is_num() ) { return traverse_tree ( node->rhand ); }
    return traverse_tree ( node->lhand );
}
//NOLINTEND(misc-no-recursion)
} // namespace pfme