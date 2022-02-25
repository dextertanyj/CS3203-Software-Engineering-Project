#include "Common/ExpressionProcessor/VariableNode.h"

#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::VariableNode Invalid Name Test") {
	REQUIRE_THROWS_AS(VariableNode("0A"), ExpressionProcessorException);
	REQUIRE_THROWS_AS(VariableNode("123"), ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::equals") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");

	SECTION(" Same Object Test") { REQUIRE(variable->equals(variable)); }

	SECTION("Common::ExpressionProcessor::VariableNode::equals Same Variable Name Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
		REQUIRE(variable->equals(other));
	}

	SECTION("Common::ExpressionProcessor::VariableNode::equals Different Variable Name Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
		REQUIRE_FALSE(variable->equals(other));
	}

	SECTION("Common::ExpressionProcessor::VariableNode::equals Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<ConstantNode>("123");
		shared_ptr<ExpressionNode> other_operator =
			make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<VariableNode>("B"), make_unique<VariableNode>("C"));
		REQUIRE_FALSE(variable->equals(other));
		REQUIRE_FALSE(variable->equals(other_operator));
	}
}

TEST_CASE("Common::ExpressionProcessor::VariableNode::contains") {
	shared_ptr<VariableNode> variable = make_shared<VariableNode>("A");

	SECTION("Same Object Test") { REQUIRE(variable->contains(variable)); }

	SECTION("Same Variable Name Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
		REQUIRE(variable->contains(other));
	}

	SECTION("Different Variable Name Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
		REQUIRE_FALSE(variable->contains(other));
	}

	SECTION("Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<ConstantNode>("123");
		shared_ptr<ExpressionNode> contained =
			make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<VariableNode>("A"), make_unique<VariableNode>("B"));
		REQUIRE_FALSE(variable->contains(other));
		REQUIRE_FALSE(variable->contains(contained));
	}
}
