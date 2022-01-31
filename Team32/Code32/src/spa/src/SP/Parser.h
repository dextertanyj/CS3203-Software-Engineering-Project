#pragma once

#include <string>

#include "SP/AssignmentNode.h"
#include "SP/ConditionalExpressionNode.h"
#include "SP/ConstantNode.h"
#include "SP/IfNode.h"
#include "SP/Lexer.h"
#include "SP/ProcedureNode.h"
#include "SP/ProgramNode.h"
#include "SP/RelationalExpressionNode.h"
#include "SP/StatementListNode.h"
#include "SP/StatementNode.h"
#include "SP/VariableNode.h"
#include "SP/WhileNode.h"


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
