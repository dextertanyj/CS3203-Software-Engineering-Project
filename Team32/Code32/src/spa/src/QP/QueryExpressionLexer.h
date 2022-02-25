#ifndef SPA_SRC_QP_QUERYEXPRESSIONLEXER_H
#define SPA_SRC_QP_QUERYEXPRESSIONLEXER_H

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/LexerInterface.h"
#include "QP/QP.h"

class QP::QueryExpressionLexer : public Common::ExpressionProcessor::LexerInterface {
public:
	explicit QueryExpressionLexer(vector<string> expression_tokens);
	string readToken() override;
	string peekToken() override;

private:
	vector<string> expression_tokens;
	int idx;
};

#endif  // SPA_SRC_QP_QUERYEXPRESSIONLEXER_H
