#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "catch_tools.h"

#define variable(name) make_shared<VariableNode>(name)
#define constant(value) make_shared<ConstantNode>(value)
#define arithmetic(op, lhs, rhs) make_shared<ArithmeticNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::RelationalNode::equals Same Object Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	REQUIRE(op->equals(op));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::equals Same Structure Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	REQUIRE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::equals Different Operator Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::NEQ, variable("A"), constant("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::equals Different Subexpression Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(
		MathematicalOperator::EQ, arithmetic(MathematicalOperator::Plus, variable("A"), variable("B")), constant("123"));
	shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::equals Different Type Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	shared_ptr<ExpressionNode> other = variable("A");
	REQUIRE_FALSE(op->equals(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::contains Same Object Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	REQUIRE(op->contains(op));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::contains Same Structure Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::contains Different Operator Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::NEQ, variable("A"), constant("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::contains Different Subexpression Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("A"), constant("123"));
	shared_ptr<RelationalNode> other = make_shared<RelationalNode>(MathematicalOperator::EQ, variable("B"), constant("123"));
	REQUIRE_FALSE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode::contains Subexpression Test") {
	shared_ptr<RelationalNode> op = make_shared<RelationalNode>(
		MathematicalOperator::EQ, arithmetic(MathematicalOperator::Plus, variable("A"), variable("B")), constant("123"));
	shared_ptr<ExpressionNode> other = arithmetic(MathematicalOperator::Plus, variable("A"), variable("B"));
	REQUIRE(op->contains(other));
}

TEST_CASE("Common::ExpressionProcessor::RelationalNode Invalid Operator Test") {
	REQUIRE_THROWS_AS(RelationalNode(MathematicalOperator::Plus, variable("A"), constant("123")), ExpressionProcessorException);
	REQUIRE_THROWS_AS(RelationalNode(MathematicalOperator::And, variable("A"), constant("123")), ExpressionProcessorException);
}