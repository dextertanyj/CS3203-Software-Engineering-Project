#include "SP/Node/WhileNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

SP::Node::WhileNode::WhileNode(StmtRef stmt_no, unique_ptr<ExpressionNode> cond_expr, unique_ptr<StatementListNode> stmt_list)
	: StatementNode(stmt_no), cond_expr(move(cond_expr)), stmt_list(move(stmt_list)) {}

unique_ptr<SP::Node::WhileNode> SP::Node::WhileNode::parseWhileStatement(Lexer& lex, StmtRef& statement_count) {
    StmtRef statement_index = statement_count++;
    lex.nextIf("(");
	unique_ptr<ExpressionNode> condition = ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::ExpressionType::Logical);
	lex.nextIf(")");
	lex.nextIf("{");
	unique_ptr<StatementListNode> statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<WhileNode>(statement_index, move(condition), move(statements));
}

StmtRef SP::Node::WhileNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::WhileStmt);
	Common::ExpressionProcessor::Expression expression = cond_expr->extract();
	pkb.setConstant(expression.getConstants());
	pkb.setUses(stmt_ref, expression.getVariables());
	vector<StmtRef> children = stmt_list->extract(pkb);
	for (auto iter = children.begin(); iter < children.end(); ++iter) {
		pkb.setParent(stmt_ref, *iter);
	}
	return stmt_ref;
}

bool SP::Node::WhileNode::equals(const shared_ptr<StatementNode>& object) {
    shared_ptr<WhileNode> other = dynamic_pointer_cast<WhileNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->getStmtRef() == other->getStmtRef() && this->cond_expr->equals(other->cond_expr)
           && this->stmt_list->equals(other->stmt_list);
}
