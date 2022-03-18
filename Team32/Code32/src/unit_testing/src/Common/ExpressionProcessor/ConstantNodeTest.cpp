#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::TerminalNode<ConstVal>::traversal") {
	shared_ptr<TerminalNode<ConstVal>> constant = make_shared<TerminalNode<ConstVal>>(1);

	SECTION("Same Object Test") { REQUIRE_EQUALS(constant->traversal(), constant->traversal()); }

	SECTION("Same Constant Value Test") {
		shared_ptr<TerminalNode<ConstVal>> other = make_shared<TerminalNode<ConstVal>>(1);
		REQUIRE_EQUALS(constant->traversal(), other->traversal());
	}

	SECTION("Different Constant Value Test") {
		shared_ptr<TerminalNode<ConstVal>> other = make_shared<TerminalNode<ConstVal>>(2);
		REQUIRE(constant->traversal() != other->traversal());
	}

	SECTION("Different Expression Test") {
		shared_ptr<ExpressionNode> other = make_shared<TerminalNode<string>>("A");
		shared_ptr<ExpressionNode> other_operator = make_shared<ArithmeticNode>(
			MathematicalOperator::Plus, make_unique<TerminalNode<ConstVal>>(2), make_unique<TerminalNode<ConstVal>>(3));
		REQUIRE(constant->traversal() != other->traversal());
		REQUIRE(constant->traversal() != other_operator->traversal());
	}
}
