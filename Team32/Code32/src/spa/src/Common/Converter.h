#ifndef SPA_CONVERTER_H
#define SPA_CONVERTER_H

#include <string>
#include <stdexcept>

#include "Common/TypeDefs.h"

using namespace std;

namespace Converter {
struct ConversionException : public runtime_error {
	using runtime_error::runtime_error;
};
ArithmeticOperator convertArithmetic(const string& op);
RelationalOperator convertRelational(const string& op);
int convertInteger(const string& integer);
}  // namespace Converter

#endif  // SPA_CONVERTER_H
