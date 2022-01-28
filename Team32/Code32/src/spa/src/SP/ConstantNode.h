#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#include "SP/ReferenceNode.h"

class ConstantNode : public ReferenceNode {
public:
	ConstantNode(int value);
private:
	int value;
};


#endif //SPA_CONSTANTNODE_H
