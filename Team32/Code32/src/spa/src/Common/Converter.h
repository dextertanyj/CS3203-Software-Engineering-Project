#ifndef SPA_SRC_COMMON_CONVERTER_H
#define SPA_SRC_COMMON_CONVERTER_H

#include <stdexcept>
#include <string>

#include "Common/TypeDefs.h"

namespace Common::Converter {
struct ConversionException : public runtime_error {
	using runtime_error::runtime_error;
};
MathematicalOperator convertMathematical(const string& opr);
string mathematicalToString(const MathematicalOperator& opr);
ConstVal convertInteger(const string& integer);
}

#endif  // SPA_SRC_COMMON_CONVERTER_H
