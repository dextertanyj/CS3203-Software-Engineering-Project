#include "Lexer.h"
#include <regex>

using namespace std;

static regex validation_regex = regex("([^a-zA-Z0-9 \\{\\}\\(\\);=!&\\|><\\+\\-\\*\\/%\\n])");
static regex tokenization_regex = regex("([a-zA-Z][0-9a-zA-Z]*|[0-9]+|\\{|\\}|\\(|\\)|;|!={0,1}|={1,2}|&&|\\|\\||>={0,1}|<={0,1}|\\+|-|\\*|\\/|%)");

Lexer::Lexer(string source) {
    this->source = source;
    if (regex_search(source, validation_regex)) {
        throw TokenizationException("Illegal token encountered");
    }
    this->iterator = sregex_iterator(this->source.begin(), this->source.end(), tokenization_regex);
}

bool Lexer::next_token() {
    if (this->iterator != sregex_iterator()) {
        this->iterator++;
    }
    while(this->iterator != sregex_iterator()) {
        if (this->iterator->str() != "") {
            return true;
        }
        this->iterator++;
    }
    return false;
}

string Lexer::get_token() {
    return this->iterator->str();
}
