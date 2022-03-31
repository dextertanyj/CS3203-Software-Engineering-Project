#include "SP/Node/StatementNode.h"

#include "Common/Validator.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"

SP::Node::StatementNode::StatementNode(StmtRef stmt_no) : stmt_no(stmt_no) {}

StmtRef SP::Node::StatementNode::getStmtRef() const { return stmt_no; }

unique_ptr<SP::Node::StatementNode> SP::Node::StatementNode::parseStatement(Lexer& lex, StmtRef& statement_count) {
	string token = lex.readToken();
	string lookahead = lex.peekToken();
	if (lookahead == "=") {
		unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, token);
		return node;
	}
	if (token == "read") {
		return ReadNode::parseReadStatement(lex, statement_count);
	}
	if (token == "print") {
		return PrintNode::parsePrintStatement(lex, statement_count);
	}
	if (token == "call") {
		return CallNode::parseCallStatement(lex, statement_count);
	}
	if (token == "while") {
		return WhileNode::parseWhileStatement(lex, statement_count);
	}
	if (token == "if") {
		return IfNode::parseIfStatement(lex, statement_count);
	}
	throw SP::ParseException("Unknown statement type encountered: " + token + ".");
}
