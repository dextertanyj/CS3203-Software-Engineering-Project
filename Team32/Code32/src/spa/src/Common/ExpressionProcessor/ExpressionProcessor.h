#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPROCESSOR_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPROCESSOR_H

#include <memory>
#include <stdexcept>
#include <variant>

namespace Common::ExpressionProcessor {
struct ExpressionProcessorException : public std::runtime_error {
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
template <typename T>
class TerminalNode;
enum class ExpressionType { Arithmetic, Relational, Logical };
typedef std::variant<ParenthesesWrapper, std::shared_ptr<ExpressionNode>> ParenthesizedExpression;
}

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPROCESSOR_H
