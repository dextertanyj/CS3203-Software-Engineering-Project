#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/ArithmeticExpressionNode.h"
#include "SP/Node/VariableNode.h"
#include "../Node/MockArithmeticNode.h"

#include "catch.hpp"
#include "catch_tools.h"
#include <iostream>

using namespace std;

TEST_CASE("SP::Node::StatementNode::parseStatement Assignment Valid Token Test") {
    Lexer lex;
    lex.initialize("x = 10 + 3;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("x");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"10", "+", "3", ";"})));
    shared_ptr<AssignmentNode> expected =
            make_shared<AssignmentNode>(1, move(assignee), move(expression));
    REQUIRE(node->equals(move(expected)));
}

TEST_CASE("SP::Node::StatementNode::parseStatement Read Valid Token Test") {
    Lexer lex;
    lex.initialize("read x;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    shared_ptr<ReadNode> expected = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
    REQUIRE(node->equals(move(expected)));
}

TEST_CASE("SP::Node::StatementNode::parseStatement Print Valid Token Test") {
    Lexer lex;
    lex.initialize("print x;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    shared_ptr<PrintNode> expected = make_shared<PrintNode>(1, make_unique<VariableNode>("x"));
    REQUIRE(node->equals(move(expected)));
}

TEST_CASE("SP::Node::StatementNode::parseStatement Call Valid Token Test") {
    Lexer lex;
    lex.initialize("call readPoint;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    shared_ptr<CallNode> expected = make_shared<CallNode>(1, "readPoint");
    REQUIRE(node->equals(move(expected)));
}