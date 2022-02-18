#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch_tools.h"

#define variable(name) make_shared<VariableNode>(name)
#define constant(value) make_shared<ConstantNode>(value)
#define arithmetic(op, lhs, rhs) make_shared<ArithmeticNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::RelationalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(RelationalNode(MathematicalOperator::Plus, variable("A"), constant("123")), ExpressionProcessorException);
	REQUIRE_THROWS_AS(RelationalNode(MathematicalOperator::And, variable("A"), constant("123")), ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::equals ") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));

	SECTION("Same Object Test") { REQUIRE(op->equals(op)); }

	SECTION("Same Structure Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
		REQUIRE(op->equals(other));
	}

	SECTION("Different Operator Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::NEQ, variable("A"), constant("123"));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("B"), constant("123"));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE_FALSE(op->equals(other));
	}
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::contains") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));

	SECTION("Same Object Test") { REQUIRE(op->contains(op)); }

	SECTION("Same Structure Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
		REQUIRE(op->contains(other));
	}

	SECTION("Different Operator Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::NEQ, variable("A"), constant("123"));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("B"), constant("123"));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Subexpression Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE(op->contains(other));
	}
}
