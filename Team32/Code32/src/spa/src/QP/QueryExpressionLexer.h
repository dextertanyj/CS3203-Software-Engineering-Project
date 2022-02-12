#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_

#include <string>
#include <vector>

#include "Common/ArithmeticProcessor/LexerInterface.h"


class QueryExpressionLexer : public Common::ArithmeticProcessor::LexerInterface {
public:
	QueryExpressionLexer(vector<string> expressionTokens);
	string readToken();
	string peekToken();

private:
	vector<string> expressionTokens;
	int idx;
	
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_
