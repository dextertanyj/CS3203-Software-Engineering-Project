#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include <string>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"

using namespace std;

class ArithmeticProcessor::Lexer {
public:
	virtual string readToken() = 0;
	virtual string peekToken() = 0;
};

#endif  // SPA_LEXER_H
