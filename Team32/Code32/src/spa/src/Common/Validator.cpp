#include "Common/Validator.h"

#include <regex>
#include <string>

using namespace std;

regex name_regex = regex(R"((^[a-zA-Z][a-zA-Z0-9]*$))");  // NOLINT
regex integer_regex = regex(R"(^([1-9][0-9]*|0)$)");      // NOLINT

bool Common::Validator::validateArithmeticOperator(const string& opr) {
	return opr == "+" || opr == "-" || opr == "*" || opr == "/" || opr == "%";
}

bool Common::Validator::validateInteger(const string& integer) { return regex_search(integer, integer_regex); }

bool Common::Validator::validateName(const string& name) { return regex_search(name, name_regex); }