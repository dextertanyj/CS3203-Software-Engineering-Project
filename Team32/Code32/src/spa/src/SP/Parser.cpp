#include "SP/Parser.h"

#include <utility>

std::unique_ptr<SP::Node::ProgramNode> SP::Parser::parse(std::string source) {
	this->lex.initialize(move(source));
	this->statement_count = STARTING_STATEMENT_NUMBER;
	return Node::ProgramNode::parseProgram(lex, this->statement_count);
}
