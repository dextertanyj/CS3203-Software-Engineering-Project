#include "SP/Node/AssignmentNode.h"

using namespace std;
using namespace SP;

AssignmentNode::AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ArithmeticExpressionNode> expression)
	: StatementNode(stmtNo), assignee(move(assignee)), expression(move(expression)) {}

unique_ptr<AssignmentNode> AssignmentNode::parseAssignmentStatement(Lexer& lex, int& statement_count, string token) {
	unique_ptr<VariableNode> variable = VariableNode::parseVariable(std::move(token));
	lex.nextIf("=");
	unique_ptr<ArithmeticExpressionNode> expression = ArithmeticExpressionNode::parseArithmeticExpression(lex);
	return make_unique<AssignmentNode>(statement_count++, move(variable), move(expression));
}

bool AssignmentNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<AssignmentNode> other = dynamic_pointer_cast<AssignmentNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->stmtNo == other->stmtNo && this->assignee->equals(move(other->assignee))
        && this->expression->equals(move(other->expression));
}