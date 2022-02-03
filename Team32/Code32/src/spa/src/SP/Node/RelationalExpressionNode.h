#ifndef SPA_RELATIONALEXPRESSIONNODE_H
#define SPA_RELATIONALEXPRESSIONNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/RelationalFactorNode.h"

using namespace std;

class RelationalExpressionNode : public ConditionalExpressionNode {
public:
	RelationalExpressionNode(RelationalOperator op, unique_ptr<RelationalFactorNode> lhs, unique_ptr<RelationalFactorNode> rhs);
	static unique_ptr<RelationalExpressionNode> parseRelationalExpression(Lexer& lex);

private:
	RelationalOperator op;
	unique_ptr<RelationalFactorNode> lhs;
	unique_ptr<RelationalFactorNode> rhs;
};

#endif  // SPA_RELATIONALEXPRESSIONNODE_H
