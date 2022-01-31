#pragma once

#include <string>

#include "SP/Lexer.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/ConstantNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/ProcedureNode.h"
#include "SP/Node/ProgramNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"
#include "SP/Node/WhileNode.h"


using namespace std;

struct ParseException : public runtime_error {
	using runtime_error::runtime_error;
};

class Parser {
public:
	Parser();
	bool parse(string source);

private:
	Lexer lex;
	size_t statement_count;

	unique_ptr<ProgramNode> parseProgram();
	unique_ptr<ProcedureNode> parseProcedure();
	unique_ptr<StatementListNode> parseStatementList();
	unique_ptr<StatementNode> parseStatement();
	unique_ptr<AssignmentNode> parseAssignmentStatement(string token);
	unique_ptr<IfNode> parseIfStatement();
	unique_ptr<WhileNode> parseWhileStatement();
	unique_ptr<ConditionalExpressionNode> parseConditionalExpression();
	unique_ptr<RelationalExpressionNode> parseRelationalExpression();
	unique_ptr<ArithmeticExpressionNode> parseArithmeticExpression();
	unique_ptr<VariableNode> parseVariable();
        unique_ptr<VariableNode> parseVariable(string token);
	unique_ptr<ConstantNode> parseConstant();
};
