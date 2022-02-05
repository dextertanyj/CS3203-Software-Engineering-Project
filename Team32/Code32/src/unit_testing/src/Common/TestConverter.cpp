#include "Common/Converter.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("TestConverter::convertArithmetic Test") {
    REQUIRE_EQUALS(Converter::convertArithmetic("+"), ArithmeticOperator::Plus);
    REQUIRE_EQUALS(Converter::convertArithmetic("-"), ArithmeticOperator::Minus);
    REQUIRE_EQUALS(Converter::convertArithmetic("*"), ArithmeticOperator::Times);
    REQUIRE_EQUALS(Converter::convertArithmetic("/"), ArithmeticOperator::Divide);
    REQUIRE_EQUALS(Converter::convertArithmetic("%"), ArithmeticOperator::Modulo);
    REQUIRE_THROWS_AS(Converter::convertArithmetic("^"), Converter::ConversionException);
    REQUIRE_THROWS_AS(Converter::convertArithmetic("++"), Converter::ConversionException);
}

TEST_CASE("TestConverter::convertRelational Test") {
    REQUIRE_EQUALS(Converter::convertRelational(">"), RelationalOperator::GT);
    REQUIRE_EQUALS(Converter::convertRelational(">="), RelationalOperator::GTE);
    REQUIRE_EQUALS(Converter::convertRelational("<"), RelationalOperator::LT);
    REQUIRE_EQUALS(Converter::convertRelational("<="), RelationalOperator::LTE);
    REQUIRE_EQUALS(Converter::convertArithmetic("=="), RelationalOperator::EQ);
    REQUIRE_EQUALS(Converter::convertRelational("!="), RelationalOperator::NEQ);
    REQUIRE_THROWS_AS(Converter::convertRelational("!"), Converter::ConversionException);
    REQUIRE_THROWS_AS(Converter::convertRelational("+"), Converter::ConversionException);
}

TEST_CASE("TestConverter::convertInteger Test") {
    REQUIRE_EQUALS(Converter::convertInteger("1"), 1);
    REQUIRE_EQUALS(Converter::convertInteger("321"), 321);
    REQUIRE_THROWS_AS(Converter::convertInteger("abc"), Converter::ConversionException);
    REQUIRE_THROWS_AS(Converter::convertInteger("2147483649"), Converter::ConversionException);
}