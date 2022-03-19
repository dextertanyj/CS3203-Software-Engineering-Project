#include "SP/Node/IfNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

SP::Node::IfNode::IfNode(StmtRef stmt_no, unique_ptr<ExpressionNode> cond_expr, unique_ptr<StatementListNode> if_stmt_list,
                         unique_ptr<StatementListNode> else_stmt_list)
	: StatementNode(stmt_no), cond_expr(move(cond_expr)), if_stmt_list(move(if_stmt_list)), else_stmt_list(move(else_stmt_list)) {}

unique_ptr<SP::Node::IfNode> SP::Node::IfNode::parseIfStatement(Lexer& lex, StmtRef& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.nextIf("(");
	unique_ptr<ExpressionNode> condition = ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::ExpressionType::Logical);
	lex.nextIf(")");
	lex.nextIf("then");
	lex.nextIf("{");
	unique_ptr<StatementListNode> then_statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	lex.nextIf("else");
	lex.nextIf("{");
	unique_ptr<StatementListNode> else_statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<IfNode>(statement_index, move(condition), move(then_statements), move(else_statements));
}

StmtRef SP::Node::IfNode::extract(PKB::StorageUpdateInterface& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::IfStmt);
	Common::ExpressionProcessor::Expression expression = cond_expr->extract();
	pkb.setConstant(expression.getConstants());
	pkb.setUses(stmt_ref, expression.getVariables());
	pkb.setIfControl(stmt_ref, expression.getVariables());
	vector<StmtRef> then_children = if_stmt_list->extract(pkb);
	vector<StmtRef> else_children = else_stmt_list->extract(pkb);
	for (auto iter = then_children.begin(); iter < then_children.end(); ++iter) {
		pkb.setParent(stmt_ref, *iter);
	}
	for (auto iter = else_children.begin(); iter < else_children.end(); ++iter) {
		pkb.setParent(stmt_ref, *iter);
	}
	pkb.setIfNext(stmt_ref, then_children.front(), else_children.front());
	pkb.setIfExit(then_children.back(), else_children.back(), stmt_ref);
	return stmt_ref;
}

bool SP::Node::IfNode::equals(const shared_ptr<StatementNode>& object) {
	shared_ptr<IfNode> other = dynamic_pointer_cast<IfNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->getStmtRef() == other->getStmtRef() && this->cond_expr->equals(other->cond_expr) &&
	       this->if_stmt_list->equals(other->if_stmt_list) && this->else_stmt_list->equals(other->else_stmt_list);
}
