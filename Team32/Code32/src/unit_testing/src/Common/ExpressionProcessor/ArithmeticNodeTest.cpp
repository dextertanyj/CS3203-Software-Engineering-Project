#include "Common/ExpressionProcessor/ArithmeticNode.h"

#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

#define variable(name) make_shared<TerminalNode<string>>(name)
#define constant(value) make_shared<TerminalNode<ConstVal>>(value)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(ArithmeticNode(MathematicalOperator::And, variable("A"), constant(1)), ExpressionProcessorException);
	REQUIRE_THROWS_AS(ArithmeticNode(MathematicalOperator::LT, variable("A"), constant(1)), ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::ArithmeticNode::traversal") {
	shared_ptr<ArithmeticNode> op = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant(1));

	SECTION("Same Object Test") { REQUIRE_EQUALS(op->traversal(), op->traversal()); }

	SECTION("Same Structure Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("A"), constant(1));
		REQUIRE_EQUALS(op->traversal(), other->traversal());
	}

	SECTION("Different Operator Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Times, variable("A"), constant(1));
		REQUIRE(op->traversal() != other->traversal());
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<ArithmeticNode> other = make_shared<ArithmeticNode>(MathematicalOperator::Plus, variable("B"), constant(1));
		REQUIRE(op->traversal() != other->traversal());
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE(op->traversal() != other->traversal());
	}
}