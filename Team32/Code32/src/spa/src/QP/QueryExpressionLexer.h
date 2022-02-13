#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/LexerInterface.h"

using std::runtime_error;

struct QueryExpressionException : public runtime_error {
    using runtime_error::runtime_error;
};

class QueryExpressionLexer : public Common::ExpressionProcessor::LexerInterface {
public:
	QueryExpressionLexer(vector<string> expressionTokens);
	string readToken() override;
	string peekToken() override;

private:
	vector<string> expressionTokens;
	int idx;
	
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_
