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

TEST_CASE("SP::Node::ExpressionNode::extract Test") {

	SECTION("Arithmetic Expressions") {
		vector<string> tokens = vector<string>({"A", "+", "B"});
		Common::ExpressionProcessor::Expression expression = createArithmeticExpression(tokens);
		Common::ExpressionProcessor::Expression expected = createArithmeticExpression(tokens);
		ExpressionNode node = ExpressionNode(expression);
		Common::ExpressionProcessor::Expression result = node.extract();
		REQUIRE(result.equals(expression));
	}

	SECTION("Relational Expressions") {
		vector<string> tokens = vector<string>({"A", "<", "B"});
		Common::ExpressionProcessor::Expression expression = createRelationalExpression(tokens);
		Common::ExpressionProcessor::Expression expected = createRelationalExpression(tokens);
		ExpressionNode node = ExpressionNode(expression);
		Common::ExpressionProcessor::Expression result = node.extract();
		REQUIRE(result.equals(expression));
	}

	SECTION("Logical Expressions") {
		vector<string> tokens = vector<string>({"!", "(", "A", "<", "A", ")"});
		Common::ExpressionProcessor::Expression expression = createConditionalExpression(tokens);
		Common::ExpressionProcessor::Expression expected = createConditionalExpression(tokens);
		ExpressionNode node = ExpressionNode(expression);
		Common::ExpressionProcessor::Expression result = node.extract();
		REQUIRE(result.equals(expression));
	}
}