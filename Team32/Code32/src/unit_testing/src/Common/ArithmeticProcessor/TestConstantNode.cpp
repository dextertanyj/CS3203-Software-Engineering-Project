#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/BinaryOperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ArithmeticProcessor;

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::equals Same Object Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	REQUIRE(constant->equals(constant));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::equals Same Constant Value Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	shared_ptr<ConstantNode> other = make_shared<ConstantNode>("123");
	REQUIRE(constant->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::equals Different Constant Value Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	shared_ptr<ConstantNode> other = make_shared<ConstantNode>("234");
	REQUIRE_FALSE(constant->equals(other));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::equals Different Expression Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	shared_ptr<ExpressionNode> other_operator =
		make_shared<BinaryOperatorNode>(ArithmeticOperator::Plus, make_unique<ConstantNode>("234"), make_unique<ConstantNode>("345"));
	REQUIRE_FALSE(constant->equals(other));
	REQUIRE_FALSE(constant->equals(other_operator));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::contains Same Object Test") {
	shared_ptr<VariableNode> constant = make_shared<VariableNode>("123");
	REQUIRE(constant->contains(constant));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::contains Same Constant Value Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	shared_ptr<ConstantNode> other = make_shared<ConstantNode>("123");
	REQUIRE(constant->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::contains Different Constant Value Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	shared_ptr<ConstantNode> other = make_shared<ConstantNode>("234");
	REQUIRE_FALSE(constant->contains(other));
}

TEST_CASE("Common::ArithmeticProcessor::ConstantNode::contains Different Expression Test") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
	shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
	shared_ptr<ExpressionNode> other_operator =
		make_shared<BinaryOperatorNode>(ArithmeticOperator::Plus, make_unique<ConstantNode>("234"), make_unique<ConstantNode>("345"));
	shared_ptr<ExpressionNode> contained =
		make_shared<BinaryOperatorNode>(ArithmeticOperator::Plus, make_unique<ConstantNode>("123"), make_unique<ConstantNode>("234"));
	REQUIRE_FALSE(constant->contains(other));
	REQUIRE_FALSE(constant->contains(other_operator));
	REQUIRE_FALSE(constant->contains(contained));
}
