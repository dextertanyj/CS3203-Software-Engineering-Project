#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::TerminalNode<string>::traversal") {
	shared_ptr<TerminalNode<string>> variable = make_shared<TerminalNode<string>>("A");

	SECTION(" Same Object Test") { REQUIRE_EQUALS(variable->traversal(), variable->traversal()); }

	SECTION("Common::ExpressionProcessor::TerminalNode<string>::equals Same Variable Name Test") {
		shared_ptr<TerminalNode<string>> other = make_shared<TerminalNode<string>>("A");
		REQUIRE_EQUALS(variable->traversal(), other->traversal());
	}

	SECTION("Common::ExpressionProcessor::TerminalNode<string>::equals Different Variable Name Test") {
		shared_ptr<TerminalNode<string>> other = make_shared<TerminalNode<string>>("B");
		REQUIRE(variable->traversal() != other->traversal());
	}

	SECTION("Common::ExpressionProcessor::TerminalNode<string>::equals Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<TerminalNode<ConstVal>>(1);
		shared_ptr<ExpressionNode> other_operator = make_shared<ArithmeticNode>(
			MathematicalOperator::Plus, make_unique<TerminalNode<string>>("B"), make_unique<TerminalNode<string>>("C"));
		REQUIRE(variable->traversal() != other->traversal());
		REQUIRE(variable->traversal() != other_operator->traversal());
	}
}
