#ifndef SPA_SRC_SP_NODE_VARIABLENODE_H
#define SPA_SRC_SP_NODE_VARIABLENODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

using namespace std;

class SP::Node::VariableNode {
public:
	explicit VariableNode(VarRef name);
	bool equals(const shared_ptr<VariableNode>& other);
	static unique_ptr<VariableNode> parseVariable(Lexer& lex);
	static unique_ptr<VariableNode> parseVariable(string token);
	VarRef extract();

private:
	VarRef name;
};

#endif  // SPA_SRC_SP_NODE_VARIABLENODE_H
