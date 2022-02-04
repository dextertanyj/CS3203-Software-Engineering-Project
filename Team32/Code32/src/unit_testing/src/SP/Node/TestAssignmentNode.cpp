#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "../Node/MockArithmeticNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::AssignmentNode::equals Same Object Test") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    REQUIRE(node->equals(move(node)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Same Node Test") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_2), move(expression_2));
    REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different StmtNo Node Test") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(2, move(assignee_2), move(expression_2));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different Assignee Node Test") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("B");
    unique_ptr<ArithmeticExpressionNode> expression_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_2), move(expression_2));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different Expression Node Test") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_2), move(expression_2));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different Node Type Test") {
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Valid Token Test") {
    Lexer lex;
    lex.initialize("= 0;");
    int statement_count = 1;
    unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"0", ";"})));
    shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    REQUIRE(node->equals(expected));
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Complex Valid Token Test") {
    Lexer lex;
    lex.initialize("= (x + 6)");
    int statement_count = 1;
    unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"x", "+", "6", ";"})));
    shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
    REQUIRE(node->equals(expected));
}

//TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Missing Token Test") {
//    Lexer lex;
//    lex.initialize("(x + 6)");
//    int statement_count = 1;
//    AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
//    REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::ParseException);
//}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Invalid Format Test") {
    Lexer lex;
    lex.initialize("== (x + 6)");
    int statement_count = 1;
    REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
}

