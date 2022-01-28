#ifndef SPA_CONVERTER_H
#define SPA_CONVERTER_H

#include <string>

#include "Common/TypeDefs.h"

using namespace std;

struct ConversionException : public runtime_error {
	using runtime_error::runtime_error;
};

namespace Converter {
	ArithmeticOperator convertArithmetic(string op);
	RelationalOperator convertRelational(string op);
}

#endif //SPA_CONVERTER_H
