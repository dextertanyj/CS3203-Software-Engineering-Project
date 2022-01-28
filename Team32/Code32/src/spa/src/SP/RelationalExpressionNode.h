#ifndef SPA_RELATIONALEXPRESSIONNODE_H
#define SPA_RELATIONALEXPRESSIONNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "SP/RelationalFactorNode.h"

using namespace std;

class RelationalExpressionNode {
public:
	RelationalExpressionNode(RelationalOperator, unique_ptr<RelationalFactorNode>, unique_ptr<RelationalFactorNode>);

private:
	RelationalOperator op;
	unique_ptr<RelationalFactorNode> lhs;
	unique_ptr<RelationalFactorNode> rhs;
};


#endif //SPA_RELATIONALEXPRESSIONNODE_H
