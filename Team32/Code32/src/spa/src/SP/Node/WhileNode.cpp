#include "SP/Node/WhileNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

SP::Node::WhileNode::WhileNode(StmtRef stmtNo, unique_ptr<ExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), stmtLst(move(stmtLst)) {}

unique_ptr<SP::Node::WhileNode> SP::Node::WhileNode::parseWhileStatement(Lexer& lex, int& statement_count) {
    StmtRef statement_index = statement_count++;
    lex.nextIf("(");
	unique_ptr<ExpressionNode> condition = ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	lex.nextIf(")");
	lex.nextIf("{");
	unique_ptr<StatementListNode> statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<WhileNode>(statement_index, move(condition), move(statements));
}

StmtInfo SP::Node::WhileNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::WhileStmt);
	Common::ExpressionProcessor::Expression expression = condExpr->extract();
	UsageInfo usage = {expression.getVariables(), expression.getConstants()};
	for (auto iter = usage.variables.begin(); iter != usage.variables.end(); ++iter) {
		pkb.setUses(stmt_ref, *iter);
	}
	StmtInfoList children = stmtLst->extract(pkb);
	for (auto iter = children.begin(); iter < children.end(); ++iter) {
		pkb.setParent(stmt_ref, iter->get()->reference);
	}
	return {stmt_ref, StmtType::WhileStmt};
}

bool SP::Node::WhileNode::equals(const shared_ptr<StatementNode>& object) {
    shared_ptr<WhileNode> other = dynamic_pointer_cast<WhileNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->getStmtRef() == other->getStmtRef() && this->condExpr->equals(other->condExpr)
           && this->stmtLst->equals(other->stmtLst);
}
