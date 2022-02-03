#include "SP/Node/WhileNode.h"

WhileNode::WhileNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), stmtLst(move(stmtLst)) {}

unique_ptr<WhileNode> WhileNode::parseWhileStatement(Lexer& lex, int& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.next_if("(");
	unique_ptr<ConditionalExpressionNode> condition = ConditionalExpressionNode::parseConditionalExpression(lex);
	lex.next_if(")");
	lex.next_if("{");
	unique_ptr<StatementListNode> statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.next_if("}");
	return make_unique<WhileNode>(statement_index, move(condition), move(statements));
}