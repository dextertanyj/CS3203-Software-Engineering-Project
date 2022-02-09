#include "SP/Node/AndNode.h"

#include <utility>
#include "SP/SP.h"
#include "SP/Node/OrNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "../Node/MockUtilities.h"

#include "catch.hpp"
#include "catch_tools.h"
#include "MockUtilities.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::AndNode::equals Same Object Test") {
    unique_ptr<RelationalExpressionNode> lhs = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> node = make_shared<AndNode>(move(lhs), move(rhs));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::AndNode::equals Same Node Test") {
    unique_ptr<RelationalExpressionNode> lhs = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> node = make_shared<AndNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> other = make_shared<AndNode>(move(lhs_2), move(rhs_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::AndNode::equals Different Lhs Node Test") {
    unique_ptr<RelationalExpressionNode> lhs = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> node = make_shared<AndNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = createRelationalExpression("z > 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> other = make_shared<AndNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::AndNode::equals Different Rhs Node Test") {
    unique_ptr<RelationalExpressionNode> lhs = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> node = make_shared<AndNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = createRelationalExpression("z >= 0)");
    shared_ptr<AndNode> other = make_shared<AndNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::AndNode::equals Wrong Node Type Test") {
    unique_ptr<RelationalExpressionNode> lhs = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = createRelationalExpression("y == 0)");
    shared_ptr<AndNode> node = make_shared<AndNode>(move(lhs), move(rhs));
    unique_ptr<RelationalExpressionNode> lhs_2 = createRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs_2 = createRelationalExpression("z >= 0)");
    shared_ptr<OrNode> other = make_shared<OrNode>(move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("AndNode::extract Test") {
	SP::UsageInfo lhs_mock = {unordered_set<VarRef>({"A"}), unordered_set<int>({1})};
	SP::UsageInfo rhs_mock = {unordered_set<VarRef>({"B"}), unordered_set<int>({2})};
	int lhs_ctr = 0;
	int rhs_ctr = 0;
	AndNode node = AndNode(make_unique<MockCENode>(lhs_mock, lhs_ctr), make_unique<MockCENode>(rhs_mock, rhs_ctr));
	SP::UsageInfo result = node.extract();
	SP::UsageInfo expected = {unordered_set<VarRef>({"A", "B"}), unordered_set<int>({1, 2})};
	REQUIRE_EQUALS(result.constants, expected.constants);
	REQUIRE_EQUALS(result.variables, expected.variables);
	REQUIRE_EQUALS(lhs_ctr, 1);
	REQUIRE_EQUALS(rhs_ctr, 1);
}
