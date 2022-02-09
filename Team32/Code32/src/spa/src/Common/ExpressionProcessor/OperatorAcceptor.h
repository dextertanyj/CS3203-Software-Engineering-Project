#ifndef SPA_OPERATORACCEPTOR_H
#define SPA_OPERATORACCEPTOR_H

#include <string>

using namespace std;

namespace Common::ExpressionProcessor::OperatorAcceptor {
bool acceptArithmetic(string op);
bool acceptRelational(string op);
bool acceptRelationalStrict(string op);
bool acceptLogical(string op);
bool acceptUnaryLogical(string op);
bool acceptBinaryLogical(string op);
bool acceptLogicalStrict(string op);
};  // namespace Common::ExpressionProcessor::OperatorAcceptor

#endif  // SPA_OPERATORACCEPTOR_H
