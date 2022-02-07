#ifndef SPA_ARITHMETICEXPRESSIONNODE_H
#define SPA_ARITHMETICEXPRESSIONNODE_H

#include <memory>
#include <utility>

#include "SP/Lexer.h"
#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "PKB/PKB.h"
#include "SP/Lexer.h"

using namespace std;
using namespace SP;

class ArithmeticExpressionNode {
public:
	static unique_ptr<ArithmeticExpressionNode> parseArithmeticExpression(Lexer& lex);
	explicit ArithmeticExpressionNode(Common::ArithmeticProcessor::ArithmeticExpression expression);
	Common::ArithmeticProcessor::ArithmeticExpression extract();
    bool equals(shared_ptr<ArithmeticExpressionNode> object);

private:
	Common::ArithmeticProcessor::ArithmeticExpression expression;
};

#endif  // SPA_ARITHMETICEXPRESSIONNODE_H
