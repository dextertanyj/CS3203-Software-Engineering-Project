#include "SP/Parser.h"

using namespace std;

SP::Parser::Parser(SP::Lexer lex) : lex(std::move(lex)), statement_count(1) {}

unique_ptr<SP::Node::ProgramNode> SP::Parser::parse(string source) {
	this->lex.initialize(std::move(source));
	this->statement_count = 1;
	return Node::ProgramNode::parseProgram(lex, this->statement_count);
}
