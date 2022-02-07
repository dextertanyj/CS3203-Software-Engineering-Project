#ifndef SPA_RELATIONALEXPRESSIONNODE_H
#define SPA_RELATIONALEXPRESSIONNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/ArithmeticExpressionNode.h"

using namespace std;

class RelationalExpressionNode : public ConditionalExpressionNode {
public:
	RelationalExpressionNode(RelationalOperator op, unique_ptr<ArithmeticExpressionNode> lhs, unique_ptr<ArithmeticExpressionNode> rhs);
	static unique_ptr<RelationalExpressionNode> parseRelationalExpression(Lexer& lex);
	UsageInfo extract() override;
    bool equals(shared_ptr<ConditionalExpressionNode> object) override;

private:
	RelationalOperator op;
	unique_ptr<ArithmeticExpressionNode> lhs;
	unique_ptr<ArithmeticExpressionNode> rhs;
};

#endif  // SPA_RELATIONALEXPRESSIONNODE_H
