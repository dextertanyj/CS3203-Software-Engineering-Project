#ifndef SPA_RELATIONALEXPRESSIONNODE_H
#define SPA_RELATIONALEXPRESSIONNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "SP/ConditionalExpressionNode.h"
#include "SP/RelationalFactorNode.h"

using namespace std;

class RelationalExpressionNode : public ConditionalExpressionNode {
public:
	RelationalExpressionNode(RelationalOperator op, unique_ptr<RelationalFactorNode> lhs, unique_ptr<RelationalFactorNode> rhs);

private:
	RelationalOperator op;
	unique_ptr<RelationalFactorNode> lhs;
	unique_ptr<RelationalFactorNode> rhs;
};


#endif //SPA_RELATIONALEXPRESSIONNODE_H
