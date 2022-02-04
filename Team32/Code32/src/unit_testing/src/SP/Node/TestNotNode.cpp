#include "SP/Node/AndNode.h"
#include "SP/Node/NotNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "../Node/MockArithmeticNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::NotNode::equals Same Object Test") {
    unique_ptr<RelationalExpressionNode> expression = getRelationalExpression("x != 0)");
    shared_ptr<NotNode> node = make_shared<NotNode>(move(expression));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::NotNode::equals Same Node Test") {
    unique_ptr<RelationalExpressionNode> expression = getRelationalExpression("x != 0)");
    shared_ptr<NotNode> node = make_shared<NotNode>(move(expression));
    unique_ptr<RelationalExpressionNode> expression_2 = getRelationalExpression("x != 0)");
    shared_ptr<NotNode> other = make_shared<NotNode>(move(expression_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::NotNode::equals Different Node Test") {
    unique_ptr<RelationalExpressionNode> expression = getRelationalExpression("x != 0)");
    shared_ptr<NotNode> node = make_shared<NotNode>(move(expression));
    unique_ptr<RelationalExpressionNode> expression_2 = getRelationalExpression("z > 0)");
    shared_ptr<NotNode> other = make_shared<NotNode>(move(expression_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::NotNode::equals Wrong Node Type Test") {
    unique_ptr<RelationalExpressionNode> expression = getRelationalExpression("x != 0)");
    shared_ptr<NotNode> node = make_shared<NotNode>(move(expression));
    unique_ptr<RelationalExpressionNode> lhs_2 = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = getRelationalExpression("z >= 0)");
    shared_ptr<AndNode> other = make_shared<AndNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}
