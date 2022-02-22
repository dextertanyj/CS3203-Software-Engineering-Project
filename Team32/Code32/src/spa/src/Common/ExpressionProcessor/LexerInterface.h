#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LEXERINTERFACE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LEXERINTERFACE_H

#include <string>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"

using namespace std;

class Common::ExpressionProcessor::LexerInterface {
public:
	virtual string readToken() = 0;
	virtual string peekToken() = 0;
	virtual ~LexerInterface() = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LEXERINTERFACE_H
