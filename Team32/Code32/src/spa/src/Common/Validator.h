#ifndef SPA_SRC_COMMON_VALIDATOR_H
#define SPA_SRC_COMMON_VALIDATOR_H

#include <string>

namespace Common::Validator {
bool validateArithmeticOperator(const std::string& opr);
bool validateName(const std::string& name);
bool validateInteger(const std::string& integer);
}

#endif  // SPA_SRC_COMMON_VALIDATOR_H
