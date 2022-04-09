#ifndef SPA_SRC_QP_PREPROCESSOR_QUERYEXPRESSIONLEXER_H
#define SPA_SRC_QP_PREPROCESSOR_QUERYEXPRESSIONLEXER_H

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/LexerInterface.h"
#include "QP/Preprocessor/Preprocessor.h"

class QP::Preprocessor::QueryExpressionLexer : public Common::EP::LexerInterface {
public:
	explicit QueryExpressionLexer(std::vector<std::string> expression_tokens);
	std::string readToken() override;
	std::string peekToken() override;

private:
	std::vector<std::string> expression_tokens;
	size_t idx;
};

#endif  // SPA_SRC_QP_PREPROCESSOR_QUERYEXPRESSIONLEXER_H
