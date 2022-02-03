#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/OperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace ArithmeticProcessor;

TEST_CASE("VariableNode::equals Same Object Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	REQUIRE(variable->equals(variable));
}

TEST_CASE("VariableNode::equals Same Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
	REQUIRE(variable->equals(other));
}

TEST_CASE("VariableNode::equals Different Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
	REQUIRE_FALSE(variable->equals(other));
}

TEST_CASE("VariableNode::equals Different Expression Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<ExpressionNode> other = make_shared<ConstantNode>("123");
	shared_ptr<ExpressionNode> other_operator =
		make_shared<OperatorNode>(ArithmeticOperator::Plus, make_unique<VariableNode>("B"), make_unique<VariableNode>("C"));
	REQUIRE_FALSE(variable->equals(other));
	REQUIRE_FALSE(variable->equals(other_operator));
}

TEST_CASE("VariableNode::contains Same Object Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	REQUIRE(variable->contains(variable));
}

TEST_CASE("VariableNode::contains Same Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
	REQUIRE(variable->contains(other));
}

TEST_CASE("VariableNode::contains Different Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
	REQUIRE_FALSE(variable->contains(other));
}

TEST_CASE("VariableNode::contains Different Expression Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<ExpressionNode> other = make_shared<ConstantNode>("123");
	shared_ptr<ExpressionNode> other_operator =
		make_shared<OperatorNode>(ArithmeticOperator::Plus, make_unique<VariableNode>("B"), make_unique<VariableNode>("C"));
	shared_ptr<ExpressionNode> contained =
		make_shared<OperatorNode>(ArithmeticOperator::Plus, make_unique<VariableNode>("A"), make_unique<VariableNode>("B"));
	REQUIRE_FALSE(variable->contains(other));
	REQUIRE_FALSE(variable->contains(other_operator));
	REQUIRE_FALSE(variable->contains(contained));
}
