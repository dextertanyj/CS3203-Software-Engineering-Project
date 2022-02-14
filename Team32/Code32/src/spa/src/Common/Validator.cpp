#include <string>
#include <regex>

#include "Common/Validator.h"

using namespace std;

regex	name_regex = regex(R"((^[a-zA-Z][a-zA-Z0-9]*$))"); // NOLINT
regex	integer_regex = regex(R"(^([1-9][0-9]*|0)$)"); // NOLINT

bool Common::Validator::validateArithmeticOperator(const string& op) {
	return op == "+" || op == "-" || op == "*" || op == "/" || op == "%";
}

bool Common::Validator::validateInteger(const string& integer) {
	return regex_search(integer, integer_regex);
}

bool Common::Validator::validateName(const string& name) {
	return regex_search(name, name_regex);
}