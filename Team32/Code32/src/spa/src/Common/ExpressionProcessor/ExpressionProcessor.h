#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPROCESSOR_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPROCESSOR_H

#include <memory>
#include <stdexcept>
#include <variant>

#include "Common/Common.h"

namespace Common::ExpressionProcessor {
struct ExpressionProcessorException : public std::runtime_error {
	using runtime_error::runtime_error;
};

enum class ExpressionType { Arithmetic, Relational, Logical };

class ExpressionParser;
class LexerInterface;
class Expression;

class ExpressionNode;
class LogicalNode;
class BinaryLogicalNode;
class UnaryLogicalNode;
class RelationalNode;
class AtomicNode;
class ArithmeticNode;
class ParenthesesWrapper;
template <typename T>
class TerminalNode;

namespace OperatorAcceptor {};

using ParenthesizedExpression = std::variant<ParenthesesWrapper, std::shared_ptr<ExpressionNode>>;
}

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONPROCESSOR_H
