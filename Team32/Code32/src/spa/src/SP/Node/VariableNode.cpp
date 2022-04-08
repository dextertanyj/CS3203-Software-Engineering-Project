#include "SP/Node/VariableNode.h"

#include <utility>

#include "Common/Validator.h"
#include "SP/SP.h"

using namespace std;

SP::Node::VariableNode::VariableNode(VarRef name) : name(move(name)) {}

VarRef SP::Node::VariableNode::extract() const { return name; }

bool SP::Node::VariableNode::equals(const shared_ptr<VariableNode>& other) const { return other->name == this->name; }

unique_ptr<SP::Node::VariableNode> SP::Node::VariableNode::parse(SP::Lexer& lex) {
	VarRef name = lex.readToken();
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid variable name.");
	}
	return make_unique<VariableNode>(name);
}

unique_ptr<SP::Node::VariableNode> SP::Node::VariableNode::parse(string token) {
	VarRef name = move(token);
	if (!Common::Validator::validateName(name)) {
		throw SP::ParseException("Invalid variable name.");
	}
	return make_unique<VariableNode>(name);
}
