#ifndef SPA_LEXERINTERFACE_H
#define SPA_LEXERINTERFACE_H

#include <string>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"

using namespace std;

class Common::ArithmeticProcessor::LexerInterface {
public:
	virtual string readToken() = 0;
	virtual string peekToken() = 0;
};

#endif  // SPA_LEXERINTERFACE_H
