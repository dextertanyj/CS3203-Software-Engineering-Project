#ifndef SPA_ARITHMETICPROCESSOR_H
#define SPA_ARITHMETICPROCESSOR_H

#include <stdexcept>

using namespace std;

namespace ArithmeticProcessor {
struct ArithmeticProcessorException : public runtime_error {
	using runtime_error::runtime_error;
};
class Lexer;
class ArithmeticExpression;
class ExpressionNode;
class OperatorNode;
class ConstantNode;
class VariableNode;
}  // namespace ArithmeticProcessor

#endif  // SPA_ARITHMETICPROCESSOR_H
