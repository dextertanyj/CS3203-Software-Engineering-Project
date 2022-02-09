#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/BinaryOperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ArithmeticProcessor;

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::equals Same Object Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::equals Same Structure Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<BinaryOperatorNode> other = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::equals Different Operator Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<BinaryOperatorNode> other = make_shared<BinaryOperatorNode>(MathematicalOperator::Times, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::equals Different Subexpression Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<BinaryOperatorNode> other = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("B"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::equals Different Type Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<VariableNode>("A"));
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::contains Same Object Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<VariableNode>("A"));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::contains Same Structure Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<BinaryOperatorNode> other = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::contains Different Operator Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<BinaryOperatorNode> other = make_shared<BinaryOperatorNode>(MathematicalOperator::Times, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::contains Different Subexpression Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<BinaryOperatorNode> other = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("B"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::BinaryOperatorNode::contains Subexpression Test") {
	shared_ptr<BinaryOperatorNode> op = make_shared<BinaryOperatorNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	REQUIRE(op->contains(other));
}
