#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include "Common/TypeDefs.h"
#include "SP/Node/ReferenceNode.h"

class VariableNode : public ReferenceNode {
public:
	explicit VariableNode(VarRef name);

private:
	VarRef name;
};

#endif //SPA_VARIABLENODE_H
