#include "Common/Converter.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch_tools.h"

#define variable(name) make_shared<VariableNode>(name)
#define constant(value) make_shared<ConstantNode>(value)
#define relational(op, lhs, rhs) make_shared<RelationalNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::equals Same Object Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::equals Same Structure Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<BinaryLogicalNode> other =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::equals Different Operator Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<BinaryLogicalNode> other =
		make_shared<BinaryLogicalNode>(MathematicalOperator::Or, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::equals Different Subexpression Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<BinaryLogicalNode> other =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::NEQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::equals Different Type Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<ExpressionNode> other = variable("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::contains Same Object Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::contains Same Structure Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<BinaryLogicalNode> other =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::contains Different Operator Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<BinaryLogicalNode> other =
		make_shared<BinaryLogicalNode>(MathematicalOperator::Or, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::contains Different Subexpression Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<BinaryLogicalNode> other =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::NEQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode::contains Subexpression Test") {
	shared_ptr<BinaryLogicalNode> op =
		make_shared<BinaryLogicalNode>(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                   relational(MathematicalOperator::EQ, variable("B"), constant("456")));
	shared_ptr<ExpressionNode> other = relational(MathematicalOperator::EQ, variable("B"), constant("456"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::BinaryLogicalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(BinaryLogicalNode(MathematicalOperator::Plus, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                    relational(MathematicalOperator::EQ, variable("B"), constant("456"))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(BinaryLogicalNode(MathematicalOperator::LT, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                    relational(MathematicalOperator::EQ, variable("B"), constant("456"))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(BinaryLogicalNode(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")),
	                                    relational(MathematicalOperator::EQ, variable("B"), constant("456"))),
	                  ExpressionProcessorException);
}
