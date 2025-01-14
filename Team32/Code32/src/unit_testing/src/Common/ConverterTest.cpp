#include "Common/Converter.h"

#include <climits>

#include "catch_tools.h"

TEST_CASE("Common::Converter::convertMathematical Test") {
	REQUIRE_EQUALS(Common::Converter::convertMathematical("+"), MathematicalOperator::Plus);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("-"), MathematicalOperator::Minus);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("*"), MathematicalOperator::Times);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("/"), MathematicalOperator::Divide);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("%"), MathematicalOperator::Modulo);
	REQUIRE_EQUALS(Common::Converter::convertMathematical(">"), MathematicalOperator::GT);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("<"), MathematicalOperator::LT);
	REQUIRE_EQUALS(Common::Converter::convertMathematical(">="), MathematicalOperator::GTE);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("<="), MathematicalOperator::LTE);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("=="), MathematicalOperator::EQ);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("!="), MathematicalOperator::NEQ);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("&&"), MathematicalOperator::And);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("||"), MathematicalOperator::Or);
	REQUIRE_EQUALS(Common::Converter::convertMathematical("!"), MathematicalOperator::Not);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("&"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("|"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("="), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("^"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertMathematical("++"), Common::Converter::ConversionException);
}

TEST_CASE("Common::Converter::convertInteger Test") {
	REQUIRE_EQUALS(Common::Converter::convertInteger("0"), 0);
	REQUIRE_EQUALS(Common::Converter::convertInteger("1"), 1);
	REQUIRE_EQUALS(Common::Converter::convertInteger("321"), 321);
	REQUIRE_EQUALS(Common::Converter::convertInteger("18446744073709551615"), ULLONG_MAX);
	REQUIRE_THROWS_AS(Common::Converter::convertInteger("18446744073709551616"), Common::Converter::ConversionException);
	REQUIRE_THROWS_AS(Common::Converter::convertInteger("abc"), Common::Converter::ConversionException);
}