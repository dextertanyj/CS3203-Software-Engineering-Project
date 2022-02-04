#ifndef SPA_CONDITIONALEXPRESSIONNODE_H
#define SPA_CONDITIONALEXPRESSIONNODE_H

#include <memory>

#include "SP/Lexer.h"

using namespace std;
using namespace SP;

class ConditionalExpressionNode {
public:
	static unique_ptr<ConditionalExpressionNode> parseConditionalExpression(Lexer& lex);
};

#endif  // SPA_CONDITIONALEXPRESSIONNODE_H
