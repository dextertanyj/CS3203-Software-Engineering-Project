#ifndef SPA_SRC_COMMON_VALIDATOR_H
#define SPA_SRC_COMMON_VALIDATOR_H

#include <string>

using namespace std;

namespace Common::Validator {
bool validateArithmeticOperator(const string& op);
bool validateName(const string& name);
bool validateInteger(const string& integer);
}

#endif  // SPA_SRC_COMMON_VALIDATOR_H
