#include "SP/Node/StatementNode.h"

#include <string>

#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/PrintReadNode.tpp"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"

#define READ "read"
#define PRINT "print"
#define CALL "call"
#define WHILE "while"
#define IF "if"

using namespace std;

SP::Node::StatementNode::StatementNode(StmtRef stmt_no) : stmt_no(stmt_no) {}

StmtRef SP::Node::StatementNode::getStmtRef() const { return stmt_no; }

unique_ptr<SP::Node::StatementNode> SP::Node::StatementNode::parse(Lexer& lex, StmtRef& statement_count) {
	string token = lex.readToken();
	string lookahead = lex.peekToken();
	if (lookahead == EQUALS) {
		return AssignmentNode::parse(lex, statement_count, token);
	}
	if (token == READ) {
		return PrintReadNode<StmtType::Read>::parse(lex, statement_count);
	}
	if (token == PRINT) {
		return PrintReadNode<StmtType::Print>::parse(lex, statement_count);
	}
	if (token == CALL) {
		return CallNode::parse(lex, statement_count);
	}
	if (token == WHILE) {
		return WhileNode::parse(lex, statement_count);
	}
	if (token == IF) {
		return IfNode::parse(lex, statement_count);
	}
	throw SP::ParseException("Unknown statement type encountered: " + token + ".");
}
