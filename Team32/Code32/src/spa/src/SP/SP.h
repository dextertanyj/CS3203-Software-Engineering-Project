#ifndef SPA_SP_H
#define SPA_SP_H

#include <stdexcept>

using namespace std;

namespace SP {
struct ParseException : public runtime_error {
	using runtime_error::runtime_error;
};
struct TokenizationException : public runtime_error {
	using runtime_error::runtime_error;
};
class Lexer;
class Parser;
class DesignExtractor;
}  // namespace SP

#endif  // SPA_SP_H
