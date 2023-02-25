#include <algorithm>
#include <iostream>
#include <iterator>
#include <locale>
#include <memory>
#include <pfme/Lexer.hpp>
#include <pfme/Parser.hpp>
#include <pfme/Visitor.hpp>
#include <string>
#include <string_view>
#include <variant>
#include <vector>


// "5 * 3 * 3 + ( ( 4.3 + 3 * 5 ) + 24 ) * 3 / 7"

int main ( int argc, char** argv )
{
    setlocale ( LC_ALL, "en_US.UTF-8" ); // NOLINT(concurrency-mt-unsafe)
    std::cout << "\n\tParser for Mathematical expressions - written by Johannes Konstantin Post\n"
              << "\tFor questions and feedback consult the readme, use --help or -h for more options, q to quit\n"
              << "\t------------------------------------ Copyright(C) 2023 ------------------------------------\n\n";
    std::vector<std::string_view> args ( argc ); // NOLINT(cppcorequidelines-init-variables)
    for ( int i = 0; i < argc; ++i ) { args.emplace_back ( *std::next ( argv, static_cast<ptrdiff_t> ( i ) ) ); }

    bool debugInfo     = false;
    bool verboseOutput = false;
    if ( std::find ( args.begin(), args.end(), "-d" ) != args.end() ||
         std::find ( args.begin(), args.end(), "--debug" ) != args.end() )
    {
        debugInfo = true;
    }
    if ( std::find ( args.begin(), args.end(), "-v" ) != args.end() ||
         std::find ( args.begin(), args.end(), "--verbose" ) != args.end() )
    {
        verboseOutput = true;
    }
    if ( std::find ( args.begin(), args.end(), "-h" ) != args.end() ||
         std::find ( args.begin(), args.end(), "--help" ) != args.end() )
    {
        std::cout << "\nPossible arguments are:\n"
                  << "\t-d, --debug\tactivate debug mode for more verbose output\n"
                  << "\t-h, --help\tshow all possible arguments\n\n";
        return 0;
    }

    while ( true )
    {
        std::cout << "$ = ";

        std::string input {};
        std::getline ( std::cin, input );

        if ( std::all_of ( input.begin(), input.end(), ::isspace ) ) { continue; }
        if ( input == "q" || input == "Q" ) { break; }

        std::unique_ptr<pfme::Lexer> lexer { std::make_unique<pfme::Lexer> (
            input ) }; // NOLINT(cppcorequidelines-init-variables)

        try
        {
            std::unique_ptr<pfme::Parser>        parser { std::make_unique<pfme::Parser> (
                std::move ( lexer ) ) }; // NOLINT(cppcorequidelines-init-variables)
            const std::unique_ptr<pfme::Visitor> visitor { std::make_unique<pfme::Visitor> (
                std::move ( parser ) ) }; // NOLINT(cppcorequidelines-init-variables)
            if ( debugInfo )
            {
                visitor->set_debug_mode ( true );
                visitor->print_tree();
                auto res = visitor->visit();
                std::cout << '\n' << input << " = " << res;
            }
            else { std::cout << "" << visitor->visit(); }
        }
        catch ( const std::exception& e )
        {
            if ( verboseOutput ) { std::cerr << '\n' << e.what(); }
            else { std::cerr << "illformed expression"; }
        }
        std::cout << '\n';
    }
}
