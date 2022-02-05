#include <utility>

#include "SP/Parser.h"
#include "catch.hpp"

using namespace std;
using namespace SP;

TEST_CASE("Parser::parse") {
	Parser parser = Parser();
	parser.parse(
		"procedure main {\n"
		"    read x;\n"
		"    read y;\n"
		"    if = while + then + else;\n"
		"    if (x == y) then {\n"
		"    while (x > 0) {\n"
		"        print x;\n"
		"        x = x - 1;\n"
		"    }\n"
		"    } else {\n"
		"    print x; print y;\n"
		"    }\n"
		"}");
}
