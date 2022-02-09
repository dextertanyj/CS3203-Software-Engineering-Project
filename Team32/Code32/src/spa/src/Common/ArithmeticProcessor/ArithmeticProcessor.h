#ifndef SPA_ARITHMETICPROCESSOR_H
#define SPA_ARITHMETICPROCESSOR_H

#include <stdexcept>

using namespace std;

namespace Common::ArithmeticProcessor {
struct ArithmeticProcessorException : public runtime_error {
	using runtime_error::runtime_error;
};
class LexerInterface;
class ArithmeticExpression;
class ExpressionNode;
class BinaryOperatorNode;
class UnaryOperatorNode;
class ConstantNode;
class VariableNode;
}  // namespace ArithmeticProcessor

#endif  // SPA_ARITHMETICPROCESSOR_H
