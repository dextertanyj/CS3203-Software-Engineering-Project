#include "SP/Parser.h"

#include <utility>

unique_ptr<SP::Node::ProgramNode> SP::Parser::parse(string source) {
	this->lex.initialize(move(source));
	this->statement_count = STARTING_STATEMENT_NUMBER;
	return Node::ProgramNode::parseProgram(lex, this->statement_count);
}
