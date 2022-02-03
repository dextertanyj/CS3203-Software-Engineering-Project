#include "SP/Parser.h"

#include <memory>

#include "SP/Node/ProgramNode.h"
#include "SP/ParseException.h"

using namespace std;

Parser::Parser() : statement_count(1) {}

bool Parser::parse(string source) {
	try {
		this->lex.initialize(std::move(source));
	} catch (TokenizationException&) {
		return false;
	}
	this->statement_count = 1;
	try {
		unique_ptr<ProgramNode> program = ProgramNode::parseProgram(lex, this->statement_count);
	} catch (ParseException&) {
		return false;
	}
	return true;
}
