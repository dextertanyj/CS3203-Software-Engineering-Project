#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include "Common/TypeDefs.h"
#include "SP/Lexer.h"

using namespace std;
using namespace SP;

class VariableNode {
public:
	explicit VariableNode(VarRef name);
	static unique_ptr<VariableNode> parseVariable(Lexer& lex);
	static unique_ptr<VariableNode> parseVariable(string token);

private:
	VarRef name;
};

#endif  // SPA_VARIABLENODE_H
