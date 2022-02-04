#include "SP/Node/WhileNode.h"

WhileNode::WhileNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), stmtLst(move(stmtLst)) {}

unique_ptr<WhileNode> WhileNode::parseWhileStatement(Lexer& lex, int& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.nextIf("(");
	unique_ptr<ConditionalExpressionNode> condition = ConditionalExpressionNode::parseConditionalExpression(lex);
	lex.nextIf(")");
	lex.nextIf("{");
	unique_ptr<StatementListNode> statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<WhileNode>(statement_index, move(condition), move(statements));
}

bool WhileNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<WhileNode> other = dynamic_pointer_cast<WhileNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->stmtNo == other->stmtNo && this->condExpr->equals(move(other->condExpr))
           && this->stmtLst->equals(move(other->stmtLst));
}