#include "SP/Node/AssignmentNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

SP::Node::AssignmentNode::AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ExpressionNode> expression)
	: StatementNode(stmtNo), assignee(move(assignee)), expression(move(expression)) {}

unique_ptr<SP::Node::AssignmentNode> SP::Node::AssignmentNode::parseAssignmentStatement(Lexer& lex, int& statement_count, string token) {
	unique_ptr<VariableNode> variable = VariableNode::parseVariable(std::move(token));
	lex.nextIf("=");
	unique_ptr<ExpressionNode> expression =
		ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	lex.nextIf(";");
	return make_unique<AssignmentNode>(statement_count++, move(variable), move(expression));
}

StmtRef SP::Node::AssignmentNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Assign);
	VarRef lhs = assignee->extract();
	pkb.setModifies(stmt_ref, lhs);
	Common::ExpressionProcessor::Expression rhs = expression->extract();
	pkb.setConstant(rhs.getConstants());
	pkb.setUses(stmt_ref, rhs.getVariables());
	pkb.setAssign(stmt_ref, lhs, rhs);
	return stmt_ref;
}

bool SP::Node::AssignmentNode::equals(const shared_ptr<StatementNode>& object) {
	shared_ptr<AssignmentNode> other = dynamic_pointer_cast<AssignmentNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->getStmtRef() == other->getStmtRef() && this->assignee->equals(other->assignee) &&
	       this->expression->equals(other->expression);
}
