#ifndef SPA_SRC_SP_SP_H
#define SPA_SRC_SP_SP_H

#include <stdexcept>

namespace SP {
struct ParseException : public std::runtime_error {
	using runtime_error::runtime_error;
};

struct TokenizationException : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class Processor;
class Lexer;
class Parser;
class DesignExtractor;

namespace Node {}
}

#endif  // SPA_SRC_SP_SP_H
