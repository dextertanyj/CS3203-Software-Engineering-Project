#include "SP/Node/RelationalExpressionNode.h"

#include "ArithmeticExpressionNode.h"
#include "Common/Converter.h"

RelationalExpressionNode::RelationalExpressionNode(RelationalOperator op, unique_ptr<ArithmeticExpressionNode> lhs,
                                                   unique_ptr<ArithmeticExpressionNode> rhs)
	: op(op), lhs(move(lhs)), rhs(move(rhs)) {}

unique_ptr<RelationalExpressionNode> RelationalExpressionNode::parseRelationalExpression(Lexer& lex) {
	unique_ptr<ArithmeticExpressionNode> lhs = ArithmeticExpressionNode::parseArithmeticExpression(lex);
	RelationalOperator op = Converter::convertRelational(lex.readToken());
	unique_ptr<ArithmeticExpressionNode> rhs = ArithmeticExpressionNode::parseArithmeticExpression(lex);
	return make_unique<RelationalExpressionNode>(op, move(lhs), move(rhs));
}

//bool RelationalExpressionNode::equals(shared_ptr<ConditionalExpressionNode> object) {
//    shared_ptr<RelationalExpressionNode> other = dynamic_pointer_cast<RelationalExpressionNode>(object);
//    if (other == nullptr) {
//        return false;
//    }
//    return this->op == other->op && this->lhs->equals(move(other->lhs)) &&
//        this->rhs->equals(move(other->rhs));
//}
