#include <pfme/Visitor.hpp>

namespace pfme
{
Visitor::Visitor ( std::unique_ptr<Parser>&& parser )
    : m_parser ( std::move ( parser ) )
{
    m_parser->parse();
}

auto Visitor::print_tree() const -> void { this->m_parser->print_binary_tree ( m_parser->get_root().get() ); }

auto Visitor::visit() -> num_t
{
    auto root = this->m_parser->get_root();
    while ( !root->is_num() )
    {
        auto                                  operation = traverse_tree ( root );
        std::function<num_t ( num_t, num_t )> functor;

        switch ( operation->m_type )
        {
        case AST_TYPE::MULTIPLICATION:
            functor = [] ( num_t factor1, num_t factor2 ) constexpr -> num_t {
                num_t product;
                std::visit ( [&] ( auto fac1, auto fac2 ) { product = fac1 * fac2; }, factor1, factor2 );
                return product;
            };
            break;
        case AST_TYPE::DIVISION:
            functor = [] ( num_t dividend, num_t divisor ) constexpr -> num_t {
                num_t quotient;
                std::visit (
                    [&] ( auto div1, auto div2 ) {
                        if ( div2 == 0 ) { throw std::runtime_error ( "Division by 0" ); }
                        quotient = static_cast<long double> ( div1 ) / div2;
                    },
                    dividend,
                    divisor );
                return quotient;
            };
            break;
        case AST_TYPE::ADDITION:
            functor = [] ( num_t summand1, num_t summand2 ) constexpr -> num_t {
                num_t sum;
                std::visit ( [&] ( auto add1, auto add2 ) { sum = add1 + add2; }, summand1, summand2 );
                return sum;
            };
            break;
        case AST_TYPE::SUBTRACTION:
            functor = [] ( num_t minuend, num_t subtrahend ) constexpr -> num_t {
                num_t difference;
                std::visit ( [&] ( auto sub1, auto sub2 ) { difference = sub1 - sub2; }, minuend, subtrahend );
                return difference;
            };
            break;
        default: break;
        }
        perform_operation ( operation, functor );
    }
    return root->value_for_type();
}

//NOLINTBEGIN(misc-no-recursion)
auto Visitor::traverse_tree ( std::shared_ptr<AST> node ) -> std::shared_ptr<AST>
{
    if ( node->lhand->is_num() && node->rhand->is_num() ) { return node; }
    if ( node->lhand->is_num() ) { return traverse_tree ( node->rhand ); }
    return traverse_tree ( node->lhand );
}
//NOLINTEND(misc-no-recursion)

auto Visitor::perform_operation ( const std::shared_ptr<AST>&            operation,
                                  std::function<num_t ( num_t, num_t )>& bin_operator ) const -> void
{
    if ( this->m_debug_mode )
    {
        std::cout << "performing operation: " << operation->lhand->operation_to_string() << ' '
                  << operation->operation_to_string() << ' ' << operation->rhand->operation_to_string() << " = ";
    }
    num_t result = bin_operator ( operation->lhand->value_for_type(), operation->rhand->value_for_type() );
    if ( operation->lhand->m_type == AST_TYPE::FLOAT || operation->rhand->m_type == AST_TYPE::FLOAT ||
         operation->m_type == AST_TYPE::DIVISION )
    {
        operation->m_type  = AST_TYPE::FLOAT;
        operation->m_float = std::get<long double> ( result );
        if ( this->m_debug_mode ) { std::cout << operation->m_float << '\n'; }
        return;
    }
    operation->m_type    = AST_TYPE::INTEGER;
    operation->m_integer = std::get<long long int> ( result );
    if ( this->m_debug_mode ) { std::cout << operation->m_integer << '\n'; }
}
} // namespace pfme