#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::equals Same Object Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::equals Same Structure Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::equals Different Operator Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Times, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::equals Different Subexpression Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("B"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::equals Different Type Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<VariableNode>("A"));
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::contains Same Object Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<VariableNode>("A"));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::contains Same Structure Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::contains Different Operator Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Times, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::contains Different Subexpression Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("B"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryOperatorNode::contains Subexpression Test") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	REQUIRE(op->contains(other));
}
