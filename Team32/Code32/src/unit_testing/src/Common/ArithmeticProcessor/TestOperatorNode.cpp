#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/OperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ArithmeticProcessor;

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::equals Same Object Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::equals Same Structure Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<OperatorNode> other = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::equals Different Operator Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<OperatorNode> other = make_shared<OperatorNode>(ArithmeticOperator::Times, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::equals Different Subexpression Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<OperatorNode> other = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("B"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::equals Different Type Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<VariableNode>("A"));
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::contains Same Object Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<VariableNode>("A"));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::contains Same Structure Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<OperatorNode> other = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::contains Different Operator Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<OperatorNode> other = make_shared<OperatorNode>(ArithmeticOperator::Times, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::contains Different Subexpression Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<OperatorNode> other = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("B"), make_shared<ConstantNode>("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::OperatorNode::contains Subexpression Test") {
	shared_ptr<OperatorNode> op = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("123"));
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	REQUIRE(op->contains(other));
}
