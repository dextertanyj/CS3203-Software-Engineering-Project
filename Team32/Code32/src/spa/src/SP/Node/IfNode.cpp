#include "SP/Node/IfNode.h"

using namespace std;

IfNode::IfNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst,
               unique_ptr<StatementListNode> elseStmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), ifStmtLst(move(ifStmtLst)), elseStmtLst(move(elseStmtLst)) {}

unique_ptr<IfNode> IfNode::parseIfStatement(Lexer& lex, int& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.next_if("(");
	unique_ptr<ConditionalExpressionNode> condition = ConditionalExpressionNode::parseConditionalExpression(lex);
	lex.next_if("(");
	lex.next_if("then");
	lex.next_if("{");
	unique_ptr<StatementListNode> then_statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.next_if("}");
	lex.next_if("else");
	lex.next_if("{");
	unique_ptr<StatementListNode> else_statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.next_if("}");
	return make_unique<IfNode>(statement_index, move(condition), move(then_statements), move(else_statements));
}