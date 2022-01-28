#ifndef SPA_RELATIONALEXPRESSIONNODE_H
#define SPA_RELATIONALEXPRESSIONNODE_H

#include "Common/TypeDefs.h"
#include "SP/RelationalFactorNode.h"

class RelationalExpressionNode {
public:
	RelationalExpressionNode(RelationalOperator, RelationalFactorNode, RelationalFactorNode);

private:
	RelationalOperator op;
	RelationalFactorNode lhs;
	RelationalFactorNode rhs;
};


#endif //SPA_RELATIONALEXPRESSIONNODE_H
