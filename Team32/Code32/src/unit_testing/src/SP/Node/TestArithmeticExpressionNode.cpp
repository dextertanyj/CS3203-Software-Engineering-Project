#include "SP/Node/ArithmeticExpressionNode.h"
#include "../Node/MockUtilities.h"
#include "../../Common/ArithmeticProcessor/MockLexer.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::ArithmeticExpressionNode::equals Same Object Test") {
    shared_ptr<ArithmeticExpressionNode> node =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::ArithmeticExpressionNode::equals Same Node Test") {
    shared_ptr<ArithmeticExpressionNode> node =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<ArithmeticExpressionNode> other =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::ArithmeticExpressionNode::equals Same Node Test With Brackets") {
    shared_ptr<ArithmeticExpressionNode> node =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<ArithmeticExpressionNode> other =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"(", "A", ")", ";"})));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::ArithmeticExpressionNode::equals Different Node Test") {
    shared_ptr<ArithmeticExpressionNode> node =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<ArithmeticExpressionNode> other =
            make_shared<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"(", "B", ")", ")"})));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("ArithmeticExpressionNode::extract Test") {
	Common::ArithmeticProcessor::ArithmeticExpression expression = createArithmeticExpression(vector<string>({"A", "+", "B", ";"}));
	ArithmeticExpressionNode node = ArithmeticExpressionNode(expression);
	Common::ArithmeticProcessor::ArithmeticExpression result = node.extract();
	REQUIRE(result.equals(expression));
}