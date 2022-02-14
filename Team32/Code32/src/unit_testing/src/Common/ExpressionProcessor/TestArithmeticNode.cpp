#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch_tools.h"

#define variable(name) make_shared<VariableNode>(name)
#define constant(value) make_shared<ConstantNode>(value)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::equals Same Object Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::equals Same Structure Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ArithmeticNode> other =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::equals Different Operator Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ArithmeticNode> other =
		make_shared<ArithmeticNode>(MathematicalOperator::Times, variable("A"), constant("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::equals Different Subexpression Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ArithmeticNode> other =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("B"), constant("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::equals Different Type Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), variable("A"));
	shared_ptr<ExpressionNode> other = variable("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::contains Same Object Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), variable("A"));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::contains Same Structure Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ArithmeticNode> other =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::contains Different Operator Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ArithmeticNode> other =
		make_shared<ArithmeticNode>(MathematicalOperator::Times, variable("A"), constant("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::contains Different Subexpression Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ArithmeticNode> other =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("B"), constant("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::contains Subexpression Test") {
	shared_ptr<ArithmeticNode> op =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant("123"));
	shared_ptr<ExpressionNode> other = variable("A");
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(ArithmeticNode(MathematicalOperator::And, variable("A"), constant("123")), ExpressionProcessorException);
	REQUIRE_THROWS_AS(ArithmeticNode(MathematicalOperator::LT, variable("A"), constant("123")), ExpressionProcessorException);
}
