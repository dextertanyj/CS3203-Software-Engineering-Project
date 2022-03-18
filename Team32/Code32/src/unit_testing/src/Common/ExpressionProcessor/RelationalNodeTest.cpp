#include "Common/ExpressionProcessor/RelationalNode.h"

#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

#define variable(name) make_shared<TerminalNode<string>>(name)
#define constant(value) make_shared<TerminalNode<ConstVal>>(value)
#define arithmetic(op, lhs, rhs) make_shared<ArithmeticNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::RelationalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(RelationalNode(MathematicalOperator::Plus, variable("A"), constant(1)), ExpressionProcessorException);
	REQUIRE_THROWS_AS(RelationalNode(MathematicalOperator::And, variable("A"), constant(1)), ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::traversal") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant(1));

	SECTION("Same Object Test") { REQUIRE_EQUALS(op->traversal(), op->traversal()); }

	SECTION("Same Structure Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant(1));
		REQUIRE_EQUALS(op->traversal(), other->traversal());
	}

	SECTION("Different Operator Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::NEQ, variable("A"), constant(1));
		REQUIRE(op->traversal() != other->traversal());
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("B"), constant(1));
		REQUIRE(op->traversal() != other->traversal());
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE(op->traversal() != other->traversal());
	}
}
