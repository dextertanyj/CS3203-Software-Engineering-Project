#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::TerminalNode<string>::equals") {
	shared_ptr<TerminalNode<string>> variable = make_shared<TerminalNode<string>>("A");

	SECTION(" Same Object Test") { REQUIRE(variable->equals(variable)); }

	SECTION("Common::ExpressionProcessor::TerminalNode<string>::equals Same Variable Name Test") {
		shared_ptr<TerminalNode<string>> other = make_shared<TerminalNode<string>>("A");
		REQUIRE(variable->equals(other));
	}

	SECTION("Common::ExpressionProcessor::TerminalNode<string>::equals Different Variable Name Test") {
		shared_ptr<TerminalNode<string>> other = make_shared<TerminalNode<string>>("B");
		REQUIRE_FALSE(variable->equals(other));
	}

	SECTION("Common::ExpressionProcessor::TerminalNode<string>::equals Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<TerminalNode<ConstVal>>(1);
		shared_ptr<ExpressionNode> other_operator = make_shared<ArithmeticNode>(
			MathematicalOperator::Plus, make_unique<TerminalNode<string>>("B"), make_unique<TerminalNode<string>>("C"));
		REQUIRE_FALSE(variable->equals(other));
		REQUIRE_FALSE(variable->equals(other_operator));
	}
}

TEST_CASE("Common::ExpressionProcessor::TerminalNode<string>::contains") {
	shared_ptr<TerminalNode<string>> variable = make_shared<TerminalNode<string>>("A");

	SECTION("Same Object Test") { REQUIRE(variable->contains(variable)); }

	SECTION("Same Variable Name Test") {
		shared_ptr<TerminalNode<string>> other = make_shared<TerminalNode<string>>("A");
		REQUIRE(variable->contains(other));
	}

	SECTION("Different Variable Name Test") {
		shared_ptr<TerminalNode<string>> other = make_shared<TerminalNode<string>>("B");
		REQUIRE_FALSE(variable->contains(other));
	}

	SECTION("Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<TerminalNode<ConstVal>>(1);
		shared_ptr<ExpressionNode> contained = make_shared<ArithmeticNode>(
			MathematicalOperator::Plus, make_unique<TerminalNode<string>>("A"), make_unique<TerminalNode<string>>("B"));
		REQUIRE_FALSE(variable->contains(other));
		REQUIRE_FALSE(variable->contains(contained));
	}
}
