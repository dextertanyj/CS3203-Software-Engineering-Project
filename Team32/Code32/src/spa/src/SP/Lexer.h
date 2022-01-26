#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include <string>
#include <regex>

using namespace std;

struct TokenizationException : public std::runtime_error {
    using runtime_error::runtime_error;
};

class Lexer {
    public:
        Lexer(string);
        bool next_token();
        string get_token();

    private:
        string source;
        sregex_iterator iterator;
};

#endif //SPA_LEXER_H
