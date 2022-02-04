#include "SP/Parser.h"

#include <memory>

#include "SP/Node/ProgramNode.h"

using namespace std;

SP::Parser::Parser() : statement_count(1) {}

unique_ptr<ProgramNode> SP::Parser::parse(string source) {
	this->lex.initialize(std::move(source));
	this->statement_count = 1;
	return ProgramNode::parseProgram(lex, this->statement_count);
}
