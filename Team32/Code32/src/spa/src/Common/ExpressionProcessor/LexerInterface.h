#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LEXERINTERFACE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LEXERINTERFACE_H

#include <string>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::LexerInterface {
public:
	virtual std::string readToken() = 0;
	virtual std::string peekToken() = 0;

	bool check(const std::string& token) {
		std::string peek = peekToken();
		return peek == token;
	}

	void match(const std::string& token) {
		std::string read = readToken();
		if (read != token) {
			throw ExpressionProcessorException("Unexpected token received: " + read + ".");
		}
	}

	virtual ~LexerInterface() = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LEXERINTERFACE_H
