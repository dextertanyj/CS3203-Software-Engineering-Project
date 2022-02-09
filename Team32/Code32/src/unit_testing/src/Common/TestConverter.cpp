#include "Common/Converter.h"
#include "catch_tools.h"

TEST_CASE("Common::Converter::convertMathematical Test") {
	REQUIRE_EQUALS(Common::Converter::convertMathematical("+"), MathematicalOperator::Plus);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("-"), MathematicalOperator::Minus);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("*"), MathematicalOperator::Times);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("/"), MathematicalOperator::Divide);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("%"), MathematicalOperator::Modulo);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("^"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("++"), Common::Converter::ConversionException);
}

TEST_CASE("Common::Converter::convertRelational Test") {
	REQUIRE_EQUALS(Common::Converter::convertRelational(">"), RelationalOperator::GT);
	REQUIRE_EQUALS(Common::Converter::convertRelational(">="), RelationalOperator::GTE);
	REQUIRE_EQUALS(Common::Converter::convertRelational("<"), RelationalOperator::LT);
	REQUIRE_EQUALS(Common::Converter::convertRelational("<="), RelationalOperator::LTE);
	REQUIRE_EQUALS(Common::Converter::convertRelational("=="), RelationalOperator::EQ);
	REQUIRE_EQUALS(Common::Converter::convertRelational("!="), RelationalOperator::NEQ);
	REQUIRE_THROWS_AS(Common::Converter::convertRelational("!"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertRelational("+"), Common::Converter::ConversionException);
}

TEST_CASE("Common::Converter::convertInteger Test") {
	REQUIRE_EQUALS(Common::Converter::convertInteger("1"), 1);
	REQUIRE_EQUALS(Common::Converter::convertInteger("321"), 321);
	REQUIRE_THROWS_AS(Common::Converter::convertInteger("abc"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertInteger("2147483649"), Common::Converter::ConversionException);
}