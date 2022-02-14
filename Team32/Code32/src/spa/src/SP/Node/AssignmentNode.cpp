#include "SP/Node/AssignmentNode.h"

using namespace std;

SP::Node::AssignmentNode::AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ArithmeticExpressionNode> expression)
	: StatementNode(stmtNo), assignee(move(assignee)), expression(move(expression)) {}

unique_ptr<SP::Node::AssignmentNode> SP::Node::AssignmentNode::parseAssignmentStatement(Lexer& lex, int& statement_count, string token) {
	unique_ptr<VariableNode> variable = VariableNode::parseVariable(std::move(token));
	lex.nextIf("=");
	unique_ptr<ArithmeticExpressionNode> expression = ArithmeticExpressionNode::parseArithmeticExpression(lex);
	lex.nextIf(";");
	return make_unique<AssignmentNode>(statement_count++, move(variable), move(expression));
}

StmtInfo SP::Node::AssignmentNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Assign);
	// TODO: Set arithmetic expression for pattern matching
	Common::ArithmeticProcessor::ArithmeticExpression rhs = expression->extract();
	pkb.setModifies(stmt_ref, assignee->extract());
	unordered_set<VarRef> variables = rhs.getVariables();
	pkb.setUses(stmt_ref, std::move(variables));
	return {stmt_ref, StmtType::Assign};
}

bool SP::Node::AssignmentNode::equals(shared_ptr<StatementNode> object) {
	shared_ptr<AssignmentNode> other = dynamic_pointer_cast<AssignmentNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->getStmtRef() == other->getStmtRef() && this->assignee->equals(move(other->assignee)) &&
	       this->expression->equals(move(other->expression));
}
