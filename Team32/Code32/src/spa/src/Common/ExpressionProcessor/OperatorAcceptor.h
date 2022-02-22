#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_OPERATORACCEPTOR_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_OPERATORACCEPTOR_H

#include <string>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

using namespace std;

typedef bool (*Acceptor)(const string& op);

namespace Common::ExpressionProcessor::OperatorAcceptor {
Acceptor getAcceptor(ExpressionType type);
bool acceptArithmetic(const string& op);
bool acceptArithmetic(MathematicalOperator op);
bool acceptRelational(const string& op);
bool acceptRelational(MathematicalOperator op);
bool acceptRelationalStrict(const string& op);
bool acceptRelationalStrict(MathematicalOperator op);
bool acceptLogical(const string& op);
bool acceptLogical(MathematicalOperator op);
bool acceptUnaryLogical(const string& op);
bool acceptUnaryLogical(MathematicalOperator op);
bool acceptBinaryLogical(const string& op);
bool acceptBinaryLogical(MathematicalOperator op);
bool acceptLogicalStrict(const string& op);
bool acceptLogicalStrict(MathematicalOperator op);
};  // namespace Common::ExpressionProcessor::OperatorAcceptor

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_OPERATORACCEPTOR_H
