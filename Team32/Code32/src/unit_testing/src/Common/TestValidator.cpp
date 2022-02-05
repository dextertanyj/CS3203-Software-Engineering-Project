#include "Common/Validator.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("TestValidator::validateArithmeticOperator Test") {
    REQUIRE(Validator::validateArithmeticOperator("+"));
    REQUIRE(Validator::validateArithmeticOperator("-"));
    REQUIRE(Validator::validateArithmeticOperator("*"));
    REQUIRE(Validator::validateArithmeticOperator("/"));
    REQUIRE(Validator::validateArithmeticOperator("%"));
    REQUIRE_FALSE(Validator::validateArithmeticOperator("^"));
    REQUIRE_FALSE(Validator::validateArithmeticOperator("++"));
}

TEST_CASE("TestValidator::validateInteger Test") {
    REQUIRE(Validator::validateInteger("0"));
    REQUIRE(Validator::validateInteger("2147483649"));
    REQUIRE_FALSE(Validator::validateInteger("123a"));
    REQUIRE_FALSE(Validator::validateInteger("-10"));
}

TEST_CASE("TestValidator::validateName Test") {
    REQUIRE(Validator::validateName("computeAverage"));
    REQUIRE(Validator::validateName("n1um"));
    REQUIRE(Validator::validateName("x"));
    REQUIRE_FALSE(Validator::validateName("1num"));
    REQUIRE_FALSE(Validator::validateName("compute_average"));
    REQUIRE_FALSE(Validator::validateName("compute-average"));
}