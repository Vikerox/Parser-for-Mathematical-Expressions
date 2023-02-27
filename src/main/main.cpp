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

struct Modes
{
    bool debugInfo     = false;
    bool verboseOutput = false;
    bool german_mode   = false;
};


auto parse_arguments ( const std::vector<std::string_view>& args ) -> Modes;

// "5 * 3 * 3 + ( ( 4.3 + 3 * 5 ) + 24 ) * 3 / 7"

int main ( int argc, char** argv )
{
    [[maybe_unused]] auto* ignore = setlocale ( LC_ALL, "en_US.UTF-8" ); // NOLINT(concurrency-mt-unsafe)
    std::cout << "\n\tParser for Mathematical expressions - written by Johannes Konstantin Post\n"
              << "\tFor questions and feedback consult the readme, use --help or -h for more options, q to quit\n"
              << "\t------------------------------------ Copyright(C) 2023 ------------------------------------\n\n";
    std::vector<std::string_view> args ( argc ); // NOLINT(cppcorequidelines-init-variables)
    for ( int i = 0; i < argc; ++i ) { args.emplace_back ( *std::next ( argv, static_cast<ptrdiff_t> ( i ) ) ); }

    auto modes = parse_arguments ( args );
    
    while ( true )
    {
        std::cout << "$ = ";

        std::string input {};
        std::getline ( std::cin, input );

        if ( std::all_of ( input.begin(), input.end(), ::isspace ) ) { continue; }
        if ( input == "q" || input == "Q" ) { break; }
        std::unique_ptr<pfme::Lexer> lexer;
        if ( !modes.german_mode )
        {
            lexer = std::make_unique<pfme::Lexer> ( input ); // NOLINT(cppcorequidelines-init-variables)
        }
        else
        {
            lexer = std::make_unique<pfme::Lexer> (
                input, ',', std::vector<char> { '.', '\'', '_' } ); // NOLINT(cppcorequidelines-init-variables)
        }

        try
        {
            const std::unique_ptr<pfme::Visitor> visitor { std::make_unique<pfme::Visitor> (
                std::move ( lexer ) ) }; // NOLINT(cppcorequidelines-init-variables)
            if ( modes.debugInfo )
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
            if ( modes.verboseOutput ) { std::cerr << '\n' << e.what(); }
            else { std::cerr << "illformed expression"; }
        }
        std::cout << '\n';
    }
}



auto parse_arguments (const std::vector<std::string_view>& args ) -> Modes
{
    Modes modes;
    if ( std::find ( args.begin(), args.end(), "-d" ) != args.end() ||
         std::find ( args.begin(), args.end(), "--debug" ) != args.end() )
    {
        modes.debugInfo = true;
    }
    if ( std::find ( args.begin(), args.end(), "-v" ) != args.end() ||
         std::find ( args.begin(), args.end(), "--verbose" ) != args.end() )
    {
        modes.verboseOutput = true;
    }
    if ( std::find ( args.begin(), args.end(), "-ger" ) != args.end() )
    {
        [[maybe_unused]] auto* ignore = setlocale ( LC_ALL, "de_DE.UTF-8" );
        modes.german_mode             = true;
    }
    if ( std::find ( args.begin(), args.end(), "-h" ) != args.end() ||
         std::find ( args.begin(), args.end(), "--help" ) != args.end() )
    {
        std::cout << "\nPossible arguments are:\n"
                  << "\t-d, --debug    activate debug mode for more verbose output\n"
                  << "\t-ger           activate german input mode (the comma seperator and the point switch roles)\n"
                  << "\t-h, --help     show all possible arguments\n\n";
        exit ( 0 );
    }
    return modes;
}
