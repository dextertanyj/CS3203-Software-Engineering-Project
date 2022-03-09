#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::TerminalNode<ConstVal>::equals") {
	shared_ptr<TerminalNode<ConstVal>> constant = make_shared<TerminalNode<ConstVal>>(1);

	SECTION("Same Object Test") { REQUIRE(constant->equals(constant)); }

	SECTION("Same Constant Value Test") {
		shared_ptr<TerminalNode<ConstVal>> other = make_shared<TerminalNode<ConstVal>>(1);
		REQUIRE(constant->equals(other));
	}

	SECTION("Different Constant Value Test") {
		shared_ptr<TerminalNode<ConstVal>> other = make_shared<TerminalNode<ConstVal>>(2);
		REQUIRE_FALSE(constant->equals(other));
	}

	SECTION("Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<TerminalNode<string>>("A");
		shared_ptr<ExpressionNode> other_operator = make_shared<ArithmeticNode>(
			MathematicalOperator::Plus, make_unique<TerminalNode<ConstVal>>(2), make_unique<TerminalNode<ConstVal>>(3));
		REQUIRE_FALSE(constant->equals(other));
		REQUIRE_FALSE(constant->equals(other_operator));
	}
}

TEST_CASE("Common::ExpressionProcessor::TerminalNode<ConstVal>::contains") {
	shared_ptr<TerminalNode<ConstVal>> constant = make_shared<TerminalNode<ConstVal>>(1);

	SECTION("Same Object Test") { REQUIRE(constant->contains(constant)); }

	SECTION("Same Constant Value Test") {
		shared_ptr<TerminalNode<ConstVal>> other = make_shared<TerminalNode<ConstVal>>(1);
		REQUIRE(constant->contains(other));
	}

	SECTION("Common::ExpressionProcessor::TerminalNode<ConstVal>::contains Different Constant Value Test") {
		shared_ptr<TerminalNode<ConstVal>> other = make_shared<TerminalNode<ConstVal>>(2);
		REQUIRE_FALSE(constant->contains(other));
	}

	SECTION("Common::ExpressionProcessor::TerminalNode<ConstVal>::contains Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<TerminalNode<string>>("A");
		shared_ptr<ExpressionNode> contained = make_shared<ArithmeticNode>(
			MathematicalOperator::Plus, make_unique<TerminalNode<ConstVal>>(1), make_unique<TerminalNode<ConstVal>>(2));
		REQUIRE_FALSE(constant->contains(other));
		REQUIRE_FALSE(constant->contains(contained));
	}
}
