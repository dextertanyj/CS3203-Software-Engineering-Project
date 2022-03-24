#ifndef SPA_SRC_SP_SP_H
#define SPA_SRC_SP_SP_H

#include <stdexcept>
#include <unordered_set>

#include "Common/TypeDefs.h"

using namespace std;

namespace SP {

struct ParseException : public runtime_error {
	using runtime_error::runtime_error;
};
struct TokenizationException : public runtime_error {
	using runtime_error::runtime_error;
};
struct InternalException : public logic_error {
	using logic_error::logic_error;
};

namespace Node {};
class Processor;
class Lexer;
class Parser;
class DesignExtractor;
}

#endif  // SPA_SRC_SP_SP_H
