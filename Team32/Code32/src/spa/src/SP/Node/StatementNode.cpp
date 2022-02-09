#include "SP/Node/StatementNode.h"

#include "Common/TypeDefs.h"
#include "Common/Validator.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/VariableNode.h"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"

StatementNode::StatementNode(StmtRef stmtNo) : stmtNo(stmtNo) {}

unique_ptr<StatementNode> StatementNode::parseStatement(Lexer& lex, int& statement_count) {
	string token = lex.readToken();
	string lookahead = lex.peekToken();
	if (lookahead == "=") {
        unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, token);
		return node;
	}
	if (token == "read") {
		// Should we abstract this at this cost of an additional function call?
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
	throw SP::ParseException("Unknown statement type encountered" + token);
}

StmtRef StatementNode::getStmtRef() const { return stmtNo; }