#include "SP/Node/RelationalExpressionNode.h"

#include "ArithmeticExpressionNode.h"
#include "Common/Converter.h"

RelationalExpressionNode::RelationalExpressionNode(RelationalOperator op, unique_ptr<ArithmeticExpressionNode> lhs,
                                                   unique_ptr<ArithmeticExpressionNode> rhs)
	: op(op), lhs(move(lhs)), rhs(move(rhs)) {}

unique_ptr<RelationalExpressionNode> RelationalExpressionNode::parseRelationalExpression(Lexer& lex) {
	unique_ptr<ArithmeticExpressionNode> lhs = ArithmeticExpressionNode::parseArithmeticExpression(lex);
	RelationalOperator op = Common::Converter::convertRelational(lex.readToken());
	unique_ptr<ArithmeticExpressionNode> rhs = ArithmeticExpressionNode::parseArithmeticExpression(lex);
	return make_unique<RelationalExpressionNode>(op, move(lhs), move(rhs));
}

UsageInfo RelationalExpressionNode::extract() {
	UsageInfo usage;
	Common::ArithmeticProcessor::ArithmeticExpression lhs_expression = lhs->extract();
	Common::ArithmeticProcessor::ArithmeticExpression rhs_expression = rhs->extract();
	unordered_set<VarRef> lhs_variables = lhs_expression.getVariables();
	unordered_set<int> lhs_constants = lhs_expression.getConstants();
	unordered_set<VarRef> rhs_variables = rhs_expression.getVariables();
	unordered_set<int> rhs_constants = rhs_expression.getConstants();
	usage.variables.insert(lhs_variables.begin(), lhs_variables.end());
	usage.variables.insert(rhs_variables.begin(), rhs_variables.end());
	usage.constants.insert(lhs_constants.begin(), lhs_constants.end());
	usage.constants.insert(rhs_constants.begin(), rhs_constants.end());
	return usage;
}

bool RelationalExpressionNode::equals(shared_ptr<ConditionalExpressionNode> object) {
	shared_ptr<RelationalExpressionNode> other = dynamic_pointer_cast<RelationalExpressionNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->op == other->op && this->lhs->equals(move(other->lhs)) && this->rhs->equals(move(other->rhs));
}
