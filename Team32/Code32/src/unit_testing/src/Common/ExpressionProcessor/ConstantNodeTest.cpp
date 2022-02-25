#include "Common/ExpressionProcessor/ConstantNode.h"

#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::ConstantNode Invalid Value Test") {
	REQUIRE_THROWS_AS(ConstantNode("0123"), ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::ConstantNode::equals") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");

	SECTION("Same Object Test") { REQUIRE(constant->equals(constant)); }

	SECTION("Same Constant Value Test") {
		shared_ptr<ConstantNode> other = make_shared<ConstantNode>("123");
		REQUIRE(constant->equals(other));
	}

	SECTION("Different Constant Value Test") {
		shared_ptr<ConstantNode> other = make_shared<ConstantNode>("234");
		REQUIRE_FALSE(constant->equals(other));
	}

	SECTION("Different Expression Test") {
		shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");
		shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
		shared_ptr<ExpressionNode> other_operator =
			make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<ConstantNode>("234"), make_unique<ConstantNode>("345"));
		REQUIRE_FALSE(constant->equals(other));
		REQUIRE_FALSE(constant->equals(other_operator));
	}
}

TEST_CASE("Common::ExpressionProcessor::ConstantNode::contains") {
	shared_ptr<ConstantNode> constant = make_shared<ConstantNode>("123");

	SECTION("Same Object Test") { REQUIRE(constant->contains(constant)); }

	SECTION("Same Constant Value Test") {
		shared_ptr<ConstantNode> other = make_shared<ConstantNode>("123");
		REQUIRE(constant->contains(other));
	}

	SECTION("Common::ExpressionProcessor::ConstantNode::contains Different Constant Value Test") {
		shared_ptr<ConstantNode> other = make_shared<ConstantNode>("234");
		REQUIRE_FALSE(constant->contains(other));
	}

	SECTION("Common::ExpressionProcessor::ConstantNode::contains Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<VariableNode>("A");
		shared_ptr<ExpressionNode> contained =
			make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_unique<ConstantNode>("123"), make_unique<ConstantNode>("234"));
		REQUIRE_FALSE(constant->contains(other));
		REQUIRE_FALSE(constant->contains(contained));
	}
}
