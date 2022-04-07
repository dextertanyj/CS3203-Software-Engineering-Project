#include "SP/Node/StatementNode.h"

#include <string>

#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/PrintReadNode.tpp"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"

using namespace std;

SP::Node::StatementNode::StatementNode(StmtRef stmt_no) : stmt_no(stmt_no) {}

StmtRef SP::Node::StatementNode::getStmtRef() const { return stmt_no; }

unique_ptr<SP::Node::StatementNode> SP::Node::StatementNode::parseStatement(Lexer& lex, StmtRef& statement_count) {
	string token = lex.readToken();
	string lookahead = lex.peekToken();
	if (lookahead == "=") {
		return AssignmentNode::parseAssignmentStatement(lex, statement_count, token);
	}
	if (token == "read") {
		return PrintReadNode<StmtType::Read>::parse(lex, statement_count);
	}
	if (token == "print") {
		return PrintReadNode<StmtType::Print>::parse(lex, statement_count);
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
