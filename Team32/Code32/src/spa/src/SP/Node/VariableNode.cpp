#include "SP/Node/VariableNode.h"

#include "Common/Validator.h"
#include "SP/SP.h"
#include "SP/Lexer.h"

VariableNode::VariableNode(VarRef name) : name(move(name)) {}

unique_ptr<VariableNode> VariableNode::parseVariable(SP::Lexer& lex) {
	VarRef name = lex.readToken();
	if (!Validator::validateName(name)) {
		throw SP::ParseException("Invalid variable name");
	}
	return make_unique<VariableNode>(name);
}

unique_ptr<VariableNode> VariableNode::parseVariable(string token) {
	VarRef name = std::move(token);
	if (!Validator::validateName(name)) {
		throw SP::ParseException("Invalid variable name");
	}
	return make_unique<VariableNode>(name);
}