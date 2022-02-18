#ifndef SPA_EXPRESSIONPROCESSOR_H
#define SPA_EXPRESSIONPROCESSOR_H

#include <stdexcept>

using namespace std;

namespace Common::ExpressionProcessor {
struct ExpressionProcessorException : public runtime_error {
	using runtime_error::runtime_error;
};
namespace OperatorAcceptor {};
class LexerInterface;
class Expression;
class ExpressionNode;
class LogicalNode;
class BinaryLogicalNode;
class UnaryLogicalNode;
class RelationalNode;
class AtomicNode;
class ArithmeticNode;
class ConstantNode;
class ParenthesesWrapper;
class VariableNode;  // NOLINT(bugprone-forward-declaration-namespace)
enum class ExpressionType { Arithmetic, Relational, Logical };
}  // namespace Common::ExpressionProcessor

#endif  // SPA_EXPRESSIONPROCESSOR_H
