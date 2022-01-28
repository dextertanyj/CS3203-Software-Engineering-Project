#include "SP/AndNode.h"
#include "SP/CallNode.h"
#include "SP/Converter.h"
#include "SP/NotNode.h"
#include "SP/OrNode.h"
#include "SP/Parser.h"
#include "SP/PrintNode.h"
#include "SP/ReadNode.h"
#include "SP/Validator.h"

using namespace std;

Parser::Parser() = default;

bool Parser::parse(string source) {
	try {
		this->lex.initialize(std::move(source));
	} catch (TokenizationException&) {
		return false;
	}
	this->statement_count = 1;
	try {
		unique_ptr<ProgramNode> program = this->parseProgram();
	} catch (ParseException&) {
		return false;
	}
	return true;
}

unique_ptr<ProgramNode> Parser::parseProgram() {
	unique_ptr<ProgramNode> program = make_unique<ProgramNode>();
	do {
		program->addProcedureNode(this->parseProcedure());
	} while (!this->lex.peek_token().empty());
	return program;
}

unique_ptr<ProcedureNode> Parser::parseProcedure() {
	this->lex.next_if("procedure");
	ProcRef name = lex.read_token();
	if (!Validator::validateName(name)) {
		throw ParseException("Invalid procedure name");
	}
	this->lex.next_if("{");
	unique_ptr<StatementListNode> statement_list = this->parseStatementList();
	this->lex.next_if("}");
	return make_unique<ProcedureNode>(name, std::move(statement_list));
}

unique_ptr<StatementListNode> Parser::parseStatementList() {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(this->parseStatement());
	} while (this->lex.peek_token() != "}");
	return statement_list;
}

unique_ptr<StatementNode> Parser::parseStatement() {
	string keyword = this->lex.peek_token();
	if (keyword == "read") {
		// Should we abstract this at this cost of an additional function call?
		// If we do, next_token() should be called by the function instead.
		this->lex.next_token();
		unique_ptr<VariableNode> variable = this->parseVariable();
		this->lex.next_if(";");
		return make_unique<ReadNode>(statement_count++, move(variable));
	}
	if (keyword == "print") {
		this->lex.next_token();
		unique_ptr<VariableNode> variable = this->parseVariable();
		this->lex.next_if(";");
		return make_unique<PrintNode>(statement_count++, move(variable));
	}
	if (keyword == "call") {
		this->lex.next_token();
		ProcRef name = this->lex.read_token();
		if (!Validator::validateName(name)) {
			throw ParseException("Invalid procedure name");
		}
		return make_unique<CallNode>(statement_count++, name);
	}
	if (keyword == "while") {
		this->lex.next_token();
		return this->parseWhileStatement();
	}
	if (keyword == "if") {
		this->lex.next_token();
		return this->parseIfStatement();
	}
	return this->parseAssignmentStatement();
}

unique_ptr<AssignmentNode> Parser::parseAssignmentStatement() {
	unique_ptr<VariableNode> variable = this->parseVariable();
	this->lex.next_if("=");
	unique_ptr<ArithmeticExpressionNode> expression = this->parseArithmeticExpression();
	return make_unique<AssignmentNode>(statement_count++, move(variable), move(expression));
}

unique_ptr<IfNode> Parser::parseIfStatement() {
	StmtRef statement_index = statement_count++;
	this->lex.next_if("(");
	unique_ptr<ConditionalExpressionNode> condition = this->parseConditionalExpression();
	this->lex.next_if("(");
	this->lex.next_if("then");
	this->lex.next_if("{");
	unique_ptr<StatementListNode> then_statements = this->parseStatementList();
	this->lex.next_if("}");
	this->lex.next_if("else");
	this->lex.next_if("{");
	unique_ptr<StatementListNode> else_statements = this->parseStatementList();
	this->lex.next_if("}");
	return make_unique<IfNode>(statement_index, move(condition), move(then_statements), move(else_statements));
}

unique_ptr<WhileNode> Parser::parseWhileStatement() {
	StmtRef statement_index = statement_count++;
	this->lex.next_if("(");
	unique_ptr<ConditionalExpressionNode> condition = this->parseConditionalExpression();
	this->lex.next_if(")");
	this->lex.next_if("{");
	unique_ptr<StatementListNode> statements = this->parseStatementList();
	this->lex.next_if("}");
	return make_unique<WhileNode>(statement_index, move(condition), move(statements));
}

unique_ptr<ConditionalExpressionNode> Parser::parseConditionalExpression() {
	string token = this->lex.peek_token();
	if (token == "!") {
		this->lex.next_if("(");
		unique_ptr<ConditionalExpressionNode> expression = this->parseConditionalExpression();
		this->lex.next_if(")");
		return make_unique<NotNode>(move(expression));
	}
	if (token == "(") {
		unique_ptr<ConditionalExpressionNode> lhs = this->parseConditionalExpression();
		this->lex.next_if(")");
		string op = this->lex.read_token();
		if (op != "&&" && op != "||") {
			throw ParseException("Invalid logical operator");
		}
		this->lex.next_if("(");
		unique_ptr<ConditionalExpressionNode> rhs = this->parseConditionalExpression();
		this->lex.next_if(")");
		if (op == "&&") {
			return make_unique<AndNode>(move(lhs), move(rhs));
		} else {
			return make_unique<OrNode>(move(lhs), move(rhs));
		}
	}
	return this->parseRelationalExpression();
}

unique_ptr<RelationalExpressionNode> Parser::parseRelationalExpression() {
	unique_ptr<RelationalFactorNode> lhs = this->parseArithmeticExpression();
	RelationalOperator op = Converter::convertRelational(this->lex.read_token());
	unique_ptr<RelationalFactorNode> rhs = this->parseArithmeticExpression();
	return make_unique<RelationalExpressionNode>(op, move(lhs), move(rhs));
}

unique_ptr<VariableNode> Parser::parseVariable() {
	VarRef name = this->lex.read_token();
	if (!Validator::validateName(name)) {
		throw ParseException("Invalid variable name");
	}
	return make_unique<VariableNode>(name);
}

unique_ptr<ConstantNode> Parser::parseConstant() {
	string token = this->lex.read_token();
	try {
		int value = Converter::convertInteger(token);
		return make_unique<ConstantNode>(value);
	} catch (ConversionException& e) {
		throw ParseException("Invalid constant value");
	}
}
