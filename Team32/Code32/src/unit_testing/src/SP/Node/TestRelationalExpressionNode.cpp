#include "Common/Converter.h"
#include "SP/Lexer.h"
#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "../Node/MockArithmeticNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::RelationalExpressionNode::equals Same Object Test") {
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    RelationalOperator op = RelationalOperator::EQ;
    shared_ptr<RelationalExpressionNode> node = make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::RelationalExpressionNode::equals Same Node Test") {
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    RelationalOperator op = RelationalOperator::EQ;
    unique_ptr<ArithmeticExpressionNode> lhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    shared_ptr<RelationalExpressionNode> node =
            make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    shared_ptr<RelationalExpressionNode> other =
            make_shared<RelationalExpressionNode>(op, move(lhs_2), move(rhs_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::RelationalExpressionNode::equals Different Op Node Test") {
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    unique_ptr<ArithmeticExpressionNode> lhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    RelationalOperator op = RelationalOperator::EQ;
    RelationalOperator op_2 = RelationalOperator::LT;
    shared_ptr<RelationalExpressionNode> node =
            make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    shared_ptr<RelationalExpressionNode> other =
            make_shared<RelationalExpressionNode>(op_2, move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::RelationalExpressionNode::equals Different Lhs Node Test") {
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    unique_ptr<ArithmeticExpressionNode> lhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"C", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    RelationalOperator op = RelationalOperator::EQ;
    shared_ptr<RelationalExpressionNode> node =
            make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    shared_ptr<RelationalExpressionNode> other =
            make_shared<RelationalExpressionNode>(op, move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::RelationalExpressionNode::equals Different Rhs Node Test") {
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"B", ";"})));
    unique_ptr<ArithmeticExpressionNode> lhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"A", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs_2 =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"C", ";"})));
    RelationalOperator op = RelationalOperator::EQ;
    shared_ptr<RelationalExpressionNode> node =
            make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    shared_ptr<RelationalExpressionNode> other =
            make_shared<RelationalExpressionNode>(op, move(lhs_2), move(rhs_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::RelationalExpressionNode::parseRelationalExpression Valid Token Test") {
    Lexer lex;
    lex.initialize("x != 0)");
    unique_ptr<RelationalExpressionNode> node = RelationalExpressionNode::parseRelationalExpression(lex);
    // COMMENT: not sure why I need the semicolon tho...
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"x", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"0", ")"})));
    RelationalOperator op = RelationalOperator::NEQ;
    shared_ptr<RelationalExpressionNode> expected =
            make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    REQUIRE(node->equals(expected));
}

TEST_CASE("SP::Node::RelationalExpressionNode::parseRelationalExpression Valid Complex Token Test") {
    Lexer lex;
    lex.initialize("(x - 2) != (5 + y * z)");
    unique_ptr<RelationalExpressionNode> node = RelationalExpressionNode::parseRelationalExpression(lex);
    unique_ptr<ArithmeticExpressionNode> lhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"x", "-", "2", ";"})));
    unique_ptr<ArithmeticExpressionNode> rhs =
            make_unique<ArithmeticExpressionNode>(getArithmeticExpression(vector<string>({"5", "+", "y", "*", "z", ")"})));
    RelationalOperator op = RelationalOperator::NEQ;
    shared_ptr<RelationalExpressionNode> expected =
            make_shared<RelationalExpressionNode>(op, move(lhs), move(rhs));
    REQUIRE(node->equals(expected));
}

TEST_CASE("SP::Node::RelationalExpressionNode::parseRelationalExpression Invalid Operator Token Test") {
    Lexer lex;
    lex.initialize("x !> 0)");
    REQUIRE_THROWS_AS(RelationalExpressionNode::parseRelationalExpression(lex), Converter::ConversionException);
}

TEST_CASE("SP::Node::RelationalExpressionNode::parseRelationalExpression Invalid Format Token Test") {
    Lexer lex;
    lex.initialize("x == != 0)");
    REQUIRE_THROWS_AS(RelationalExpressionNode::parseRelationalExpression(lex),
                      ArithmeticProcessor::ArithmeticProcessorException);
}