#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::VariableNode::equals Same Object Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	REQUIRE(variable->equals(variable));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::equals Same Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
	REQUIRE(variable->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::equals Different Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
	REQUIRE_FALSE(variable->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::equals Different Expression Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<ExpressionNode> other = make_shared<ConstantNode>("123");
	shared_ptr<ExpressionNode> other_operator =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<VariableNode>("B"), make_unique<VariableNode>("C"));
	REQUIRE_FALSE(variable->equals(other));
	REQUIRE_FALSE(variable->equals(other_operator));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::contains Same Object Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	REQUIRE(variable->contains(variable));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::contains Same Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
	REQUIRE(variable->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::contains Different Variable Name Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
	REQUIRE_FALSE(variable->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::contains Different Expression Test") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");
	shared_ptr<ExpressionNode> other = make_shared<ConstantNode>("123");
	shared_ptr<ExpressionNode> other_operator =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<VariableNode>("B"), make_unique<VariableNode>("C"));
	shared_ptr<ExpressionNode> contained =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<VariableNode>("A"), make_unique<VariableNode>("B"));
	REQUIRE_FALSE(variable->contains(other));
	REQUIRE_FALSE(variable->contains(other_operator));
	REQUIRE_FALSE(variable->contains(contained));
}

TEST_CASE("Common::ExpressionProcessor::VariableNode Invalid Name Test") {
	REQUIRE_THROWS_AS(VariableNode("0A"), ExpressionProcessorException);
}