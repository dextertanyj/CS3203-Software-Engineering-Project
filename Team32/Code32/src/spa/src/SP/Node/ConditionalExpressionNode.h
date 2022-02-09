#ifndef SPA_CONDITIONALEXPRESSIONNODE_H
#define SPA_CONDITIONALEXPRESSIONNODE_H

#include <memory>

#include "SP/Lexer.h"
#include "SP/Node/Node.h"

using namespace std;

class SP::Node::ConditionalExpressionNode {
public:
	static unique_ptr<ConditionalExpressionNode> parseConditionalExpression(Lexer& lex);
	virtual UsageInfo extract() = 0;
	virtual ~ConditionalExpressionNode() = default;
	virtual bool equals(shared_ptr<ConditionalExpressionNode> object) = 0;
};

#endif  // SPA_CONDITIONALEXPRESSIONNODE_H
