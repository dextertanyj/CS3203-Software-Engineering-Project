#ifndef SPA_SRC_SP_NODE_VARIABLENODE_H
#define SPA_SRC_SP_NODE_VARIABLENODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "SP/Lexer.h"
#include "SP/Node/Node.h"

class SP::Node::VariableNode {
public:
	explicit VariableNode(VarRef name);
	[[nodiscard]] bool equals(const shared_ptr<VariableNode>& other) const;
	[[nodiscard]] VarRef extract() const;

	static unique_ptr<VariableNode> parseVariable(Lexer& lex);
	static unique_ptr<VariableNode> parseVariable(string token);

private:
	VarRef name;
};

#endif  // SPA_SRC_SP_NODE_VARIABLENODE_H
