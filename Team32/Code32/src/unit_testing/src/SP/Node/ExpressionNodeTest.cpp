#include "SP/Node/ExpressionNode.h"
#include "../Node/MockUtilities.h"
#include "../../Common/ExpressionProcessor/MockLexer.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ExpressionNode::equals") {
    shared_ptr<ExpressionNode> node =
            make_shared<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(node));
    }

    SECTION("Same Node Test") {
        shared_ptr<ExpressionNode> other =
                make_shared<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
        REQUIRE(node->equals(other));
    }

    SECTION("Same Node Test With Brackets") {
        shared_ptr<ExpressionNode> other =
                make_shared<ExpressionNode>(createArithmeticExpression(vector<string>({"(", "A", ")", ";"})));
        REQUIRE(node->equals(other));
    }

    SECTION("Different Node Test") {
        shared_ptr<ExpressionNode> other =
                make_shared<ExpressionNode>(createArithmeticExpression(vector<string>({"(", "B", ")", ")"})));
        REQUIRE_FALSE(node->equals(other));
    }
}

TEST_CASE("ExpressionNode::extract Test") {
	Common::ExpressionProcessor::Expression expression = createArithmeticExpression(vector<string>({"A", "+", "B", ";"}));
	ExpressionNode node = ExpressionNode(expression);
	Common::ExpressionProcessor::Expression result = node.extract();
	REQUIRE(result.equals(expression));
}