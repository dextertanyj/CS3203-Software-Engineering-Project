#ifndef SPA_OPERATORACCEPTOR_H
#define SPA_OPERATORACCEPTOR_H

#include <string>

#include "Common/TypeDefs.h"

using namespace std;

namespace Common::ExpressionProcessor::OperatorAcceptor {
bool acceptArithmetic(string op);
bool acceptArithmetic(MathematicalOperator op);
bool acceptRelational(string op);
bool acceptRelational(MathematicalOperator op);
bool acceptRelationalStrict(string op);
bool acceptRelationalStrict(MathematicalOperator op);
bool acceptLogical(string op);
bool acceptLogical(MathematicalOperator op);
bool acceptUnaryLogical(string op);
bool acceptUnaryLogical(MathematicalOperator op);
bool acceptBinaryLogical(string op);
bool acceptBinaryLogical(MathematicalOperator op);
bool acceptLogicalStrict(string op);
bool acceptLogicalStrict(MathematicalOperator op);
};  // namespace Common::ExpressionProcessor::OperatorAcceptor

#endif  // SPA_OPERATORACCEPTOR_H
