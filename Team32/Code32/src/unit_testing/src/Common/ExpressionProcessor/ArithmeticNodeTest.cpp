#include "Common/ExpressionProcessor/ArithmeticNode.h"

#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

#define variable(name) make_shared<TerminalNode<string>>(name)
#define constant(value) make_shared<TerminalNode<ConstVal>>(value)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(ArithmeticNode(MathematicalOperator::And, variable("A"), constant(1)), ExpressionProcessorException);
	REQUIRE_THROWS_AS(ArithmeticNode(MathematicalOperator::LT, variable("A"), constant(1)), ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::equals Same Object Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant(1));

	SECTION("Same Object Test") { REQUIRE(op->equals(op)); }

	SECTION("Same Structure Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant(1));
		REQUIRE(op->equals(other));
	}

	SECTION("Different Operator Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Times, variable("A"), constant(1));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("B"), constant(1));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE_FALSE(op->equals(other));
	}
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::contains") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant(1));

	SECTION("Same Object Test") { REQUIRE(op->contains(op)); }

	SECTION("Same Structure Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant(1));
		REQUIRE(op->contains(other));
	}

	SECTION("Different Operator Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Times, variable("A"), constant(1));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("B"), constant(1));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Subexpression Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE(op->contains(other));
	}
}
