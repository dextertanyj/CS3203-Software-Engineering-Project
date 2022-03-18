#include "Common/ExpressionProcessor/UnaryLogicalNode.h"

#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

#define variable(name) make_shared<TerminalNode<string>>(name)
#define constant(value) make_shared<TerminalNode<ConstVal>>(value)
#define relational(op, lhs, rhs) make_shared<RelationalNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::Plus, relational(MathematicalOperator::EQ, variable("A"), constant(1))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::LT, relational(MathematicalOperator::EQ, variable("A"), constant(1))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant(1))),
	                  ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::traversal") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant(1)));

	SECTION("Same Object Test") { REQUIRE_EQUALS(op->traversal(), op->traversal()); }

	SECTION("Same Structure Test") {
		shared_ptr<UnaryLogicalNode> other =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant(1)));
		REQUIRE_EQUALS(op->traversal(), other->traversal());
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<UnaryLogicalNode> other =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::NEQ, variable("A"), constant(1)));
		REQUIRE(op->traversal() != other->traversal());
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE(op->traversal() != other->traversal());
	}
}
