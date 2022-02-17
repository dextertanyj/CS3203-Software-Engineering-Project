#ifndef SPA_CONVERTER_H
#define SPA_CONVERTER_H

#include <string>
#include <stdexcept>

#include "Common/TypeDefs.h"

using namespace std;

namespace Common::Converter {
struct ConversionException : public runtime_error {
	using runtime_error::runtime_error;
};
MathematicalOperator convertMathematical(const string& op);
ConstVal convertInteger(const string& integer);
}  // namespace Common::Converter

#endif  // SPA_CONVERTER_H
