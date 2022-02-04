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