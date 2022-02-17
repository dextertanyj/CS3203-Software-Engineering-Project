#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch_tools.h"

#define variable(name) make_shared<VariableNode>(name)
#define constant(value) make_shared<ConstantNode>(value)
#define relational(op, lhs, rhs) make_shared<RelationalNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::Plus, relational(MathematicalOperator::EQ, variable("A"), constant("123"))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::LT, relational(MathematicalOperator::EQ, variable("A"), constant("123"))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123"))),
	                  ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::equals") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));

	SECTION("Same Object Test") {
		REQUIRE(op->equals(op));
	}

	SECTION("Same Structure Test") {
		shared_ptr<UnaryLogicalNode> other =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
		REQUIRE(op->equals(other));
	}


	SECTION("Different Subexpression Test") {
		shared_ptr<UnaryLogicalNode> other =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::NEQ, variable("A"), constant("123")));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE_FALSE(op->equals(other));
	}
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::contains") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));

	SECTION("Same Object Test") {
		REQUIRE(op->contains(op));
	}

	SECTION("Same Structure Test") {
		shared_ptr<UnaryLogicalNode> other =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
		REQUIRE(op->contains(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<UnaryLogicalNode> other =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::NEQ, variable("A"), constant("123")));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Subexpression Test") {
		shared_ptr<UnaryLogicalNode> op =
			make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE(op->contains(other));
	}
}
