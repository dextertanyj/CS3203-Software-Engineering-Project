#include "../Node/MockUtilities.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/VariableNode.h"
#include "catch_tools.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::AssignmentNode::equals") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ExpressionNode> expression =
            make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    unique_ptr<VariableNode> assignee_a = make_unique<VariableNode>("A");
    unique_ptr<ExpressionNode> expression_a =
            make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(move(node)));
    }

    SECTION("Same Node Test") {
        shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_a), move(expression_a));
        REQUIRE(node->equals(move(other)));
    }

    SECTION("Different StmtNo Node Test") {
        shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(2, move(assignee_a), move(expression_a));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Different Assignee Node Test") {
        unique_ptr<VariableNode> assignee_b = make_unique<VariableNode>("B");
        shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_b), move(expression_a));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Different Expression Node Test") {
        unique_ptr<ExpressionNode> expression_b =
                make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"B", ";"})));
        shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_a), move(expression_b));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Different Node Type Test") {
        shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
        REQUIRE_FALSE(node->equals(move(other)));
    }
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement") {
    StmtRef statement_count = 1;
    SP::Lexer lex;

    SECTION("Valid Token Test") {
        lex.initialize("= (x + 6);");
        unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
        unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
        unique_ptr<ExpressionNode> expression =
                make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"(", "x", "+", "6", ")", ";"})));
        shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("Complex Valid Token Test") {
        lex.initialize("= (((x + 6) * (3 - (8))));");
        unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
        unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
        unique_ptr<ExpressionNode> expression =
                make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"(", "x", "+", "6", ")", "*", "(", "3", "-", "8", ")", ";"})));
        shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("Missing Token Test") {
        lex.initialize("(x + 6);");
        REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Missing Terminal Token Test") {
        lex.initialize("= (x + 6)");
        REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Invalid Format Test") {
        lex.initialize("== (x + 6);");
        REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Incomplete Brackets Test") {
        lex.initialize("== ((x + 6;)");
        REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }
}

TEST_CASE("AssignmentNode::extract Test") {
	SP::Lexer lex;
	PKB pkb;
	lex.initialize("1;");
	unique_ptr<ExpressionNode> expression = ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	AssignmentNode node = AssignmentNode(1, make_unique<VariableNode>("A"), std::move(expression));
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, 1);
}
