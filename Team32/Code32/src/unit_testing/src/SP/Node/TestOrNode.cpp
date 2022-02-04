#include "SP/Node/AndNode.h"
#include "SP/Node/OrNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "../Node/MockArithmeticNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::OrNode::equals Same Object Test") {
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> node = make_shared<OrNode>(move(lhs), move(rhs));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::OrNode::equals Same Node Test") {
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> node = make_shared<OrNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> other = make_shared<OrNode>(move(lhs_2), move(rhs_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::OrNode::equals Different Lhs Node Test") {
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> node = make_shared<OrNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = getRelationalExpression("z > 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> other = make_shared<OrNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::OrNode::equals Different Rhs Node Test") {
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> node = make_shared<OrNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = getRelationalExpression("z >= 0)");
    shared_ptr<OrNode> other = make_shared<OrNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::OrNode::equals Wrong Node Type Test") {
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    shared_ptr<OrNode> node = make_shared<OrNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = getRelationalExpression("z >= 0)");
    shared_ptr<AndNode> other = make_shared<AndNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}