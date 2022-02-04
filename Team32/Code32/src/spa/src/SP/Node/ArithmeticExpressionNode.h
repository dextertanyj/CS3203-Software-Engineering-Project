#ifndef SPA_ARITHMETICEXPRESSIONNODE_H
#define SPA_ARITHMETICEXPRESSIONNODE_H

#include <memory>

#include "SP/Node/RelationalFactorNode.h"
#include "SP/Lexer.h"
#include "Common/ArithmeticProcessor/ArithmeticExpression.h"

using namespace std;
using namespace SP;

class ArithmeticExpressionNode : public RelationalFactorNode {
public:
	static unique_ptr<ArithmeticExpressionNode> parseArithmeticExpression(Lexer& lex);
	ArithmeticExpressionNode(ArithmeticProcessor::ArithmeticExpression expression);

private:
	ArithmeticProcessor::ArithmeticExpression expression;
};

#endif  // SPA_ARITHMETICEXPRESSIONNODE_H
