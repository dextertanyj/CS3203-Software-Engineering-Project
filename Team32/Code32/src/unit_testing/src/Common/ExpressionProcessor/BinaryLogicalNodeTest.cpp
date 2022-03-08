#include "Common/ExpressionProcessor/BinaryLogicalNode.h"

#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "catch_tools.h"

#define variable(name) make_shared<TerminalNode<string>>(name)
#define constant(value) make_shared<TerminalNode<ConstVal>>(value)
#define relational(op, lhs, rhs) make_shared<RelationalNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(BinaryLogicalNode(MathematicalOperator::Plus, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
	                                    relational(MathematicalOperator::EQ, variable("B"), constant(2))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(BinaryLogicalNode(MathematicalOperator::LT, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
	                                    relational(MathematicalOperator::EQ, variable("B"), constant(2))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(BinaryLogicalNode(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
	                                    relational(MathematicalOperator::EQ, variable("B"), constant(2))),
	                  ExpressionProcessorException);
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::equals") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));

	SECTION("Same Object Test") { REQUIRE(op->equals(op)); }

	SECTION("Same Structure Test") {
		shared_ptr<BinaryLogicalNode> other =
			make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
		                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));
		REQUIRE(op->equals(other));
	}

	SECTION("Different Operator Test") {
		shared_ptr<BinaryLogicalNode> other =
			make_shared<BinaryLogicalNode>(MathematicalOperator::Or, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
		                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<BinaryLogicalNode> other =
			make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::NEQ, variable("A"), constant(1)),
		                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));
		REQUIRE_FALSE(op->equals(other));
	}

	SECTION("Different Type Test") {
		shared_ptr<ExpressionNode> other = variable("A");
		REQUIRE_FALSE(op->equals(other));
	}
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::contains") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));

	SECTION("Same Object Test") { REQUIRE(op->contains(op)); }

	SECTION("Same Structure Test") {
		shared_ptr<BinaryLogicalNode> other =
			make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
		                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));
		REQUIRE(op->contains(other));
	}

	SECTION("Different Operator Test") {
		shared_ptr<BinaryLogicalNode> other =
			make_shared<BinaryLogicalNode>(MathematicalOperator::Or, relational(MathematicalOperator::EQ, variable("A"), constant(1)),
		                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Different Subexpression Test") {
		shared_ptr<BinaryLogicalNode> other =
			make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::NEQ, variable("A"), constant(1)),
		                                   relational(MathematicalOperator::EQ, variable("B"), constant(2)));
		REQUIRE_FALSE(op->contains(other));
	}

	SECTION("Subexpression Test") {
		shared_ptr<ExpressionNode> other = relational(MathematicalOperator::EQ, variable("B"), constant(2));
		REQUIRE(op->contains(other));
	}
}
