#include "SP/Node/IfNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

SP::Node::IfNode::IfNode(StmtRef stmtNo, unique_ptr<ExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst,
               unique_ptr<StatementListNode> elseStmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), ifStmtLst(move(ifStmtLst)), elseStmtLst(move(elseStmtLst)) {}

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

StmtRef SP::Node::IfNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::IfStmt);
	Common::ExpressionProcessor::Expression expression = condExpr->extract();
	pkb.setConstant(expression.getConstants());
	pkb.setUses(stmt_ref, expression.getVariables());
	vector<StmtRef> then_children = ifStmtLst->extract(pkb);
	vector<StmtRef> else_children = elseStmtLst->extract(pkb);
	for (auto iter = then_children.begin(); iter < then_children.end(); ++iter) {
		pkb.setParent(stmt_ref, *iter);
	}
	for (auto iter = else_children.begin(); iter < else_children.end(); ++iter) {
		pkb.setParent(stmt_ref, *iter);
	}
	return stmt_ref;
}


bool SP::Node::IfNode::equals(const shared_ptr<StatementNode>& object) {
    shared_ptr<IfNode> other = dynamic_pointer_cast<IfNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->getStmtRef() == other->getStmtRef() && this->condExpr->equals(other->condExpr)
        && this->ifStmtLst->equals(other->ifStmtLst)
        && this->elseStmtLst->equals(other->elseStmtLst);
}
