#ifndef SPA_SP_H
#define SPA_SP_H

#include <stdexcept>
#include <vector>
#include "Common/TypeDefs.h"

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
class Processor;
struct UsageInfo {
	vector<VarRef> variables;
	vector<int> constants;
};
}  // namespace SP

#endif  // SPA_SP_H
