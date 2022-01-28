#include "SP/Converter.h"
#include "SP/Parser.h"
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

unique_ptr<ProgramNode> Parser::parseProgram() {
	unique_ptr<ProgramNode> program = make_unique<ProgramNode>();
	do {
		program->addProcedureNode(this->parseProcedure());
	} while (!this->lex.peek_token().empty());
	return program;
}

unique_ptr<StatementListNode> Parser::parseStatementList() {
	unique_ptr<StatementListNode> statement_list = make_unique<StatementListNode>();
	do {
		statement_list->addStatementNode(this->parseStatement());
	} while (this->lex.peek_token() != "}");
	return statement_list;
}
