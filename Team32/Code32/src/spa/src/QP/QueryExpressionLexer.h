#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/LexerInterface.h"

class QueryExpressionLexer : public Common::ExpressionProcessor::LexerInterface {
public:
	explicit QueryExpressionLexer(vector<string> expression_tokens);
	string readToken() override;
	string peekToken() override;

private:
	vector<string> expression_tokens;
	int idx;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYEXPRESSIONLEXER_H_
