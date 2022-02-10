#include "Common/Converter.h"
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

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::equals Same Object Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::equals Same Structure Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	shared_ptr<UnaryLogicalNode> other =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::equals Different Subexpression Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	shared_ptr<UnaryLogicalNode> other =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::NEQ, variable("A"), constant("123")));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::equals Different Type Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	shared_ptr<ExpressionNode> other = variable("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::contains Same Object Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::contains Same Structure Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	shared_ptr<UnaryLogicalNode> other =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::contains Different Subexpression Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	shared_ptr<UnaryLogicalNode> other =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::NEQ, variable("A"), constant("123")));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode::contains Subexpression Test") {
	shared_ptr<UnaryLogicalNode> op =
		make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relational(MathematicalOperator::EQ, variable("A"), constant("123")));
	shared_ptr<ExpressionNode> other = variable("A");
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::UnaryLogicalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::Plus, relational(MathematicalOperator::EQ, variable("A"), constant("123"))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::LT, relational(MathematicalOperator::EQ, variable("A"), constant("123"))),
	                  ExpressionProcessorException);
	REQUIRE_THROWS_AS(UnaryLogicalNode(MathematicalOperator::And, relational(MathematicalOperator::EQ, variable("A"), constant("123"))),
	                  ExpressionProcessorException);
}
