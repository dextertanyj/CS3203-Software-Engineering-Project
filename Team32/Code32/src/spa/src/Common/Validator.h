#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include <string>

using namespace std;

namespace Common::Validator {
bool validateArithmeticOperator(const string& op);
bool validateName(const string& name);
bool validateInteger(const string& integer);
}  // namespace Common::Validator

#endif  // SPA_VALIDATOR_H
