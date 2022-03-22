#include "Common/Validator.h"

#include "catch_tools.h"

TEST_CASE("Common::Validator::validateArithmeticOperator Test") {
	REQUIRE(Common::Validator::validateArithmeticOperator("+"));
	REQUIRE(Common::Validator::validateArithmeticOperator("-"));
	REQUIRE(Common::Validator::validateArithmeticOperator("*"));
	REQUIRE(Common::Validator::validateArithmeticOperator("/"));
	REQUIRE(Common::Validator::validateArithmeticOperator("%"));
	REQUIRE_FALSE(Common::Validator::validateArithmeticOperator("^"));
	REQUIRE_FALSE(Common::Validator::validateArithmeticOperator("++"));
}

TEST_CASE("Common::Validator::validateInteger Test") {
	REQUIRE(Common::Validator::validateInteger("0"));
	REQUIRE(Common::Validator::validateInteger("2147483649"));
	REQUIRE_FALSE(Common::Validator::validateInteger("00123"));
	REQUIRE_FALSE(Common::Validator::validateInteger("123a"));
	REQUIRE_FALSE(Common::Validator::validateInteger("-10"));
}

TEST_CASE("Common::Validator::validateName Test") {
	REQUIRE(Common::Validator::validateName("computeAverage"));
	REQUIRE(Common::Validator::validateName("ComputeAverage"));
	REQUIRE(Common::Validator::validateName("n1um"));
	REQUIRE(Common::Validator::validateName("x"));
	REQUIRE_FALSE(Common::Validator::validateName("1num"));
	REQUIRE_FALSE(Common::Validator::validateName("compute_average"));
	REQUIRE_FALSE(Common::Validator::validateName("compute-average"));
}