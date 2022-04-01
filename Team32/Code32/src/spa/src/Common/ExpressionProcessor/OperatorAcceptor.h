#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_OPERATORACCEPTOR_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_OPERATORACCEPTOR_H

#include <string>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

using Acceptor = bool (*)(const string& opr);

namespace Common::ExpressionProcessor::OperatorAcceptor {
Acceptor getAcceptor(ExpressionType type);
bool acceptArithmetic(const string& opr);
bool acceptArithmetic(MathematicalOperator opr);
bool acceptRelational(const string& opr);
bool acceptRelational(MathematicalOperator opr);
bool acceptRelationalStrict(const string& opr);
bool acceptRelationalStrict(MathematicalOperator opr);
bool acceptLogical(const string& opr);
bool acceptLogical(MathematicalOperator opr);
bool acceptUnaryLogical(const string& opr);
bool acceptUnaryLogical(MathematicalOperator opr);
bool acceptBinaryLogical(const string& opr);
bool acceptBinaryLogical(MathematicalOperator opr);
bool acceptLogicalStrict(const string& opr);
bool acceptLogicalStrict(MathematicalOperator opr);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_OPERATORACCEPTOR_H
