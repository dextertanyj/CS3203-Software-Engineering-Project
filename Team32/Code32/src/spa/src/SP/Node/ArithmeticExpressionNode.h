#ifndef SPA_ARITHMETICEXPRESSIONNODE_H
#define SPA_ARITHMETICEXPRESSIONNODE_H

#include <memory>

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

using namespace std;

class SP::Node::ArithmeticExpressionNode {
public:
	static unique_ptr<ArithmeticExpressionNode> parseArithmeticExpression(Lexer& lex);
	explicit ArithmeticExpressionNode(Common::ArithmeticProcessor::ArithmeticExpression expression);
	Common::ArithmeticProcessor::ArithmeticExpression extract();
	bool equals(shared_ptr<ArithmeticExpressionNode> object);

private:
	Common::ArithmeticProcessor::ArithmeticExpression expression;
};

#endif  // SPA_ARITHMETICEXPRESSIONNODE_H
