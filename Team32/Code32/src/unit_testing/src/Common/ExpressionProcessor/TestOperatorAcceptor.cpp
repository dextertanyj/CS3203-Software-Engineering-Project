#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "catch_tools.h"

using namespace Common::ExpressionProcessor::OperatorAcceptor;

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic String Test") {
	REQUIRE(acceptArithmetic("+"));
	REQUIRE(acceptArithmetic("-"));
	REQUIRE(acceptArithmetic("*"));
	REQUIRE(acceptArithmetic("/"));
	REQUIRE(acceptArithmetic("%"));
	REQUIRE_FALSE(acceptArithmetic("=="));
	REQUIRE_FALSE(acceptArithmetic("!="));
	REQUIRE_FALSE(acceptArithmetic("<="));
	REQUIRE_FALSE(acceptArithmetic("<"));
	REQUIRE_FALSE(acceptArithmetic(">="));
	REQUIRE_FALSE(acceptArithmetic(">"));
	REQUIRE_FALSE(acceptArithmetic("&&"));
	REQUIRE_FALSE(acceptArithmetic("||"));
	REQUIRE_FALSE(acceptArithmetic("!"));
	REQUIRE_FALSE(acceptArithmetic(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic Operator Test") {
	REQUIRE(acceptArithmetic(MathematicalOperator::Plus));
	REQUIRE(acceptArithmetic(MathematicalOperator::Minus));
	REQUIRE(acceptArithmetic(MathematicalOperator::Times));
	REQUIRE(acceptArithmetic(MathematicalOperator::Divide));
	REQUIRE(acceptArithmetic(MathematicalOperator::Modulo));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::EQ));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::NEQ));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::LTE));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::LT));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::GTE));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::GT));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::And));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::Or));
	REQUIRE_FALSE(acceptArithmetic(MathematicalOperator::Not));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict String Test") {
	REQUIRE_FALSE(acceptRelationalStrict("+"));
	REQUIRE_FALSE(acceptRelationalStrict("-"));
	REQUIRE_FALSE(acceptRelationalStrict("*"));
	REQUIRE_FALSE(acceptRelationalStrict("/"));
	REQUIRE_FALSE(acceptRelationalStrict("%"));
	REQUIRE(acceptRelationalStrict("=="));
	REQUIRE(acceptRelationalStrict("!="));
	REQUIRE(acceptRelationalStrict("<="));
	REQUIRE(acceptRelationalStrict("<"));
	REQUIRE(acceptRelationalStrict(">="));
	REQUIRE(acceptRelationalStrict(">"));
	REQUIRE_FALSE(acceptRelationalStrict("&&"));
	REQUIRE_FALSE(acceptRelationalStrict("||"));
	REQUIRE_FALSE(acceptRelationalStrict("!"));
	REQUIRE_FALSE(acceptRelationalStrict(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict Operator Test") {
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Plus));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Minus));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Times));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Divide));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Modulo));
	REQUIRE(acceptRelationalStrict(MathematicalOperator::EQ));
	REQUIRE(acceptRelationalStrict(MathematicalOperator::NEQ));
	REQUIRE(acceptRelationalStrict(MathematicalOperator::LTE));
	REQUIRE(acceptRelationalStrict(MathematicalOperator::LT));
	REQUIRE(acceptRelationalStrict(MathematicalOperator::GTE));
	REQUIRE(acceptRelationalStrict(MathematicalOperator::GT));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::And));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Or));
	REQUIRE_FALSE(acceptRelationalStrict(MathematicalOperator::Not));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptRelational String Test") {
	REQUIRE(acceptRelational("+"));
	REQUIRE(acceptRelational("-"));
	REQUIRE(acceptRelational("*"));
	REQUIRE(acceptRelational("/"));
	REQUIRE(acceptRelational("%"));
	REQUIRE(acceptRelational("=="));
	REQUIRE(acceptRelational("!="));
	REQUIRE(acceptRelational("<="));
	REQUIRE(acceptRelational("<"));
	REQUIRE(acceptRelational(">="));
	REQUIRE(acceptRelational(">"));
	REQUIRE_FALSE(acceptRelational("&&"));
	REQUIRE_FALSE(acceptRelational("||"));
	REQUIRE_FALSE(acceptRelational("!"));
	REQUIRE_FALSE(acceptRelational(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptRelational Operator Test") {
	REQUIRE(acceptRelational(MathematicalOperator::Plus));
	REQUIRE(acceptRelational(MathematicalOperator::Minus));
	REQUIRE(acceptRelational(MathematicalOperator::Times));
	REQUIRE(acceptRelational(MathematicalOperator::Divide));
	REQUIRE(acceptRelational(MathematicalOperator::Modulo));
	REQUIRE(acceptRelational(MathematicalOperator::EQ));
	REQUIRE(acceptRelational(MathematicalOperator::NEQ));
	REQUIRE(acceptRelational(MathematicalOperator::LTE));
	REQUIRE(acceptRelational(MathematicalOperator::LT));
	REQUIRE(acceptRelational(MathematicalOperator::GTE));
	REQUIRE(acceptRelational(MathematicalOperator::GT));
	REQUIRE_FALSE(acceptRelational(MathematicalOperator::And));
	REQUIRE_FALSE(acceptRelational(MathematicalOperator::Or));
	REQUIRE_FALSE(acceptRelational(MathematicalOperator::Not));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical String Test") {
	REQUIRE_FALSE(acceptUnaryLogical("+"));
	REQUIRE_FALSE(acceptUnaryLogical("-"));
	REQUIRE_FALSE(acceptUnaryLogical("*"));
	REQUIRE_FALSE(acceptUnaryLogical("/"));
	REQUIRE_FALSE(acceptUnaryLogical("%"));
	REQUIRE_FALSE(acceptUnaryLogical("=="));
	REQUIRE_FALSE(acceptUnaryLogical("!="));
	REQUIRE_FALSE(acceptUnaryLogical("<="));
	REQUIRE_FALSE(acceptUnaryLogical("<"));
	REQUIRE_FALSE(acceptUnaryLogical(">="));
	REQUIRE_FALSE(acceptUnaryLogical(">"));
	REQUIRE_FALSE(acceptUnaryLogical("&&"));
	REQUIRE_FALSE(acceptUnaryLogical("||"));
	REQUIRE(acceptUnaryLogical("!"));
	REQUIRE_FALSE(acceptUnaryLogical(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical Operator Test") {
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::Plus));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::Minus));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::Times));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::Divide));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::Modulo));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::EQ));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::NEQ));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::LTE));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::LT));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::GTE));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::GT));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::And));
	REQUIRE_FALSE(acceptUnaryLogical(MathematicalOperator::Or));
	REQUIRE(acceptUnaryLogical(MathematicalOperator::Not));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical String Test") {
	REQUIRE_FALSE(acceptBinaryLogical("+"));
	REQUIRE_FALSE(acceptBinaryLogical("-"));
	REQUIRE_FALSE(acceptBinaryLogical("*"));
	REQUIRE_FALSE(acceptBinaryLogical("/"));
	REQUIRE_FALSE(acceptBinaryLogical("%"));
	REQUIRE_FALSE(acceptBinaryLogical("=="));
	REQUIRE_FALSE(acceptBinaryLogical("!="));
	REQUIRE_FALSE(acceptBinaryLogical("<="));
	REQUIRE_FALSE(acceptBinaryLogical("<"));
	REQUIRE_FALSE(acceptBinaryLogical(">="));
	REQUIRE_FALSE(acceptBinaryLogical(">"));
	REQUIRE(acceptBinaryLogical("&&"));
	REQUIRE(acceptBinaryLogical("||"));
	REQUIRE_FALSE(acceptBinaryLogical("!"));
	REQUIRE_FALSE(acceptBinaryLogical(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical Operator Test") {
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::Plus));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::Minus));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::Times));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::Divide));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::Modulo));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::EQ));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::NEQ));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::LTE));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::LT));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::GTE));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::GT));
	REQUIRE(acceptBinaryLogical(MathematicalOperator::And));
	REQUIRE(acceptBinaryLogical(MathematicalOperator::Or));
	REQUIRE_FALSE(acceptBinaryLogical(MathematicalOperator::Not));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict String Test") {
	REQUIRE_FALSE(acceptLogicalStrict("+"));
	REQUIRE_FALSE(acceptLogicalStrict("-"));
	REQUIRE_FALSE(acceptLogicalStrict("*"));
	REQUIRE_FALSE(acceptLogicalStrict("/"));
	REQUIRE_FALSE(acceptLogicalStrict("%"));
	REQUIRE_FALSE(acceptLogicalStrict("=="));
	REQUIRE_FALSE(acceptLogicalStrict("!="));
	REQUIRE_FALSE(acceptLogicalStrict("<="));
	REQUIRE_FALSE(acceptLogicalStrict("<"));
	REQUIRE_FALSE(acceptLogicalStrict(">="));
	REQUIRE_FALSE(acceptLogicalStrict(">"));
	REQUIRE(acceptLogicalStrict("&&"));
	REQUIRE(acceptLogicalStrict("||"));
	REQUIRE(acceptLogicalStrict("!"));
	REQUIRE_FALSE(acceptLogicalStrict(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict Operator Test") {
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::Plus));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::Minus));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::Times));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::Divide));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::Modulo));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::EQ));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::NEQ));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::LTE));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::LT));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::GTE));
	REQUIRE_FALSE(acceptLogicalStrict(MathematicalOperator::GT));
	REQUIRE(acceptLogicalStrict(MathematicalOperator::And));
	REQUIRE(acceptLogicalStrict(MathematicalOperator::Or));
	REQUIRE(acceptLogicalStrict(MathematicalOperator::Not));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptLogical String Test") {
	REQUIRE(acceptLogical("+"));
	REQUIRE(acceptLogical("-"));
	REQUIRE(acceptLogical("*"));
	REQUIRE(acceptLogical("/"));
	REQUIRE(acceptLogical("%"));
	REQUIRE(acceptLogical("=="));
	REQUIRE(acceptLogical("!="));
	REQUIRE(acceptLogical("<="));
	REQUIRE(acceptLogical("<"));
	REQUIRE(acceptLogical(">="));
	REQUIRE(acceptLogical(">"));
	REQUIRE(acceptLogical("&&"));
	REQUIRE(acceptLogical("||"));
	REQUIRE(acceptLogical("!"));
	REQUIRE_FALSE(acceptLogical(")"));
}

TEST_CASE("Common::ExpressionProcessor::OperatorAcceptor::acceptLogical Operator Test") {
	REQUIRE(acceptLogical(MathematicalOperator::Plus));
	REQUIRE(acceptLogical(MathematicalOperator::Minus));
	REQUIRE(acceptLogical(MathematicalOperator::Times));
	REQUIRE(acceptLogical(MathematicalOperator::Divide));
	REQUIRE(acceptLogical(MathematicalOperator::Modulo));
	REQUIRE(acceptLogical(MathematicalOperator::EQ));
	REQUIRE(acceptLogical(MathematicalOperator::NEQ));
	REQUIRE(acceptLogical(MathematicalOperator::LTE));
	REQUIRE(acceptLogical(MathematicalOperator::LT));
	REQUIRE(acceptLogical(MathematicalOperator::GTE));
	REQUIRE(acceptLogical(MathematicalOperator::GT));
	REQUIRE(acceptLogical(MathematicalOperator::And));
	REQUIRE(acceptLogical(MathematicalOperator::Or));
	REQUIRE(acceptLogical(MathematicalOperator::Not));
}