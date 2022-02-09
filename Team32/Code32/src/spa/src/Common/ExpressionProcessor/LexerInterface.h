#ifndef SPA_LEXERINTERFACE_H
#define SPA_LEXERINTERFACE_H

#include <string>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"

using namespace std;

class Common::ExpressionProcessor::LexerInterface {
public:
	virtual string readToken() = 0;
	virtual string peekToken() = 0;
};

#endif  // SPA_LEXERINTERFACE_H
