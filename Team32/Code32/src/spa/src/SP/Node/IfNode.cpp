#include "SP/Node/IfNode.h"

using namespace std;

SP::Node::IfNode::IfNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst,
               unique_ptr<StatementListNode> elseStmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), ifStmtLst(move(ifStmtLst)), elseStmtLst(move(elseStmtLst)) {}

unique_ptr<SP::Node::IfNode> SP::Node::IfNode::parseIfStatement(Lexer& lex, int& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.nextIf("(");
	unique_ptr<ConditionalExpressionNode> condition = ConditionalExpressionNode::parseConditionalExpression(lex);
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

StmtInfo SP::Node::IfNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::IfStmt);
	StmtInfoList then_children = ifStmtLst->extract(pkb);
	StmtInfoList else_children = elseStmtLst->extract(pkb);
	for (auto iter = then_children.begin(); iter < then_children.end(); ++iter) {
		pkb.setParent(stmt_ref, iter->reference);
	}
	for (auto iter = else_children.begin(); iter < else_children.end(); ++iter) {
		pkb.setParent(stmt_ref, iter->reference);
	}
	UsageInfo usage = condExpr->extract();
	for (auto iter = usage.variables.begin(); iter != usage.variables.end(); ++iter) {
		pkb.setUses(stmt_ref, *iter);
	}
	return {stmt_ref, StmtType::IfStmt};
}


bool SP::Node::IfNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<IfNode> other = dynamic_pointer_cast<IfNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->getStmtRef() == other->getStmtRef() && this->condExpr->equals(move(other->condExpr))
        && this->ifStmtLst->equals(move(other->ifStmtLst))
        && this->elseStmtLst->equals(move(other->elseStmtLst));
}
