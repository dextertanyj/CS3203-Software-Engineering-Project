#include "SP/Node/VariableNode.h"

#include "SP/ParseException.h"
#include "SP/Validator.h"

VariableNode::VariableNode(VarRef name) : name(move(name)) {}

unique_ptr<VariableNode> VariableNode::parseVariable(Lexer& lex) {
	VarRef name = lex.read_token();
	if (!Validator::validateName(name)) {
		throw ParseException("Invalid variable name");
	}
	return make_unique<VariableNode>(name);
}

unique_ptr<VariableNode> VariableNode::parseVariable(string token) {
	VarRef name = std::move(token);
	if (!Validator::validateName(name)) {
		throw ParseException("Invalid variable name");
	}
	return make_unique<VariableNode>(name);
}