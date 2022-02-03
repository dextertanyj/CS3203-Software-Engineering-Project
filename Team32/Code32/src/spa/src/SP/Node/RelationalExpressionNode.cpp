#include "SP/Node/RelationalExpressionNode.h"

#include "ArithmeticExpressionNode.h"
#include "Common/Converter.h"

RelationalExpressionNode::RelationalExpressionNode(RelationalOperator op, unique_ptr<RelationalFactorNode> lhs,
                                                   unique_ptr<RelationalFactorNode> rhs)
	: op(op), lhs(move(lhs)), rhs(move(rhs)) {}

unique_ptr<RelationalExpressionNode> RelationalExpressionNode::parseRelationalExpression(Lexer& lex) {
	unique_ptr<RelationalFactorNode> lhs = ArithmeticExpressionNode::parseArithmeticExpression();
	RelationalOperator op = Converter::convertRelational(lex.read_token());
	unique_ptr<RelationalFactorNode> rhs = ArithmeticExpressionNode::parseArithmeticExpression();
	return make_unique<RelationalExpressionNode>(op, move(lhs), move(rhs));
}
