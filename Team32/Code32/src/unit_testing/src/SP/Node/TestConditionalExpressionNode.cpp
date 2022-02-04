#include "Common/Converter.h"
#include "SP/Lexer.h"
#include "SP/SP.h"
#include "SP/Node/AndNode.h"
#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/NotNode.h"
#include "SP/Node/OrNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "../Node/MockArithmeticNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression NotNode Valid Token Test") {
    Lexer lex;
    lex.initialize("!(x != 0)");
    unique_ptr<ConditionalExpressionNode> node = ConditionalExpressionNode::parseConditionalExpression(lex);
    unique_ptr<RelationalExpressionNode> expression = getRelationalExpression("x != 0)");
    unique_ptr<NotNode> expected = make_unique<NotNode>(move(expression));
    REQUIRE(expected->equals(move(node)));
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression NotNode Missing Brackets Token Test") {
    Lexer lex;
    lex.initialize("!x != 0)");
    REQUIRE_THROWS_AS(ConditionalExpressionNode::parseConditionalExpression(lex), SP::TokenizationException);
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression NotNode Invalid Cond Token Test") {
    Lexer lex;
    lex.initialize("!(x)");
    REQUIRE_THROWS_AS(ConditionalExpressionNode::parseConditionalExpression(lex), Converter::ConversionException);
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression AndNode Valid Token Test") {
    Lexer lex;
    lex.initialize("(x != 0) && (y == 0)");
    unique_ptr<ConditionalExpressionNode> node = ConditionalExpressionNode::parseConditionalExpression(lex);
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    unique_ptr<AndNode> expected = make_unique<AndNode>(move(lhs), move(rhs));
    REQUIRE(expected->equals(move(node)));
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression AndNode Complex Valid Token Test") {
    Lexer lex;
    lex.initialize("(x != 0) && ((y == 0) && (z == (5 + x)))");
    unique_ptr<ConditionalExpressionNode> node = ConditionalExpressionNode::parseConditionalExpression(lex);
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> lhs_inner = getRelationalExpression("y == 0)");
    unique_ptr<RelationalExpressionNode> rhs_inner = getRelationalExpression("z == (5 + x))");
    unique_ptr<AndNode> rhs = make_unique<AndNode>(move(lhs_inner), move(rhs_inner));
    unique_ptr<AndNode> expected = make_unique<AndNode>(move(lhs), move(rhs));
    REQUIRE(expected->equals(move(node)));
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression AndNode Missing Bracket Test") {
    Lexer lex;
    lex.initialize("(x != 0) && y == 0)");
    REQUIRE_THROWS_AS(ConditionalExpressionNode::parseConditionalExpression(lex), SP::TokenizationException);
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression AndNode Invalid Condition Test") {
    Lexer lex;
    lex.initialize("(x != 0) && y");
//    REQUIRE_THROWS_AS(ConditionalExpressionNode::parseConditionalExpression(lex), SP::ParseException);
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression OrNode Valid Token Test") {
    Lexer lex;
    lex.initialize("(x != 0) || (y == 0)");
    unique_ptr<ConditionalExpressionNode> node = ConditionalExpressionNode::parseConditionalExpression(lex);
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> rhs = getRelationalExpression("y == 0)");
    unique_ptr<OrNode> expected = make_unique<OrNode>(move(lhs), move(rhs));
    REQUIRE(expected->equals(move(node)));
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression OrNode Complex Valid Token Test") {
    Lexer lex;
    lex.initialize("(x != 0) || ((y == 0) || (z == (5 + x)))");
    unique_ptr<ConditionalExpressionNode> node = ConditionalExpressionNode::parseConditionalExpression(lex);
    unique_ptr<RelationalExpressionNode> lhs = getRelationalExpression("x != 0)");
    unique_ptr<RelationalExpressionNode> lhs_inner = getRelationalExpression("y == 0)");
    unique_ptr<RelationalExpressionNode> rhs_inner = getRelationalExpression("z == (5 + x))");
    unique_ptr<OrNode> rhs = make_unique<OrNode>(move(lhs_inner), move(rhs_inner));
    unique_ptr<OrNode> expected = make_unique<OrNode>(move(lhs), move(rhs));
    REQUIRE(expected->equals(move(node)));
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression OrNode Missing Bracket Test") {
    Lexer lex;
    lex.initialize("(x != 0) || y == 0)");
    REQUIRE_THROWS_AS(ConditionalExpressionNode::parseConditionalExpression(lex), SP::TokenizationException);
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression OrNode Invalid Condition Test") {
    Lexer lex;
    lex.initialize("(x != 0) || y");
//    REQUIRE_THROWS_AS(ConditionalExpressionNode::parseConditionalExpression(lex), SP::ParseException);
}

TEST_CASE("SP::Node::ConditionalExpressionNode::parseConditionalExpression Complex Valid Token Test") {
    Lexer lex;
    lex.initialize("(!(x != 0)) && ((y == 0) || (z == (5 + x)))");
    unique_ptr<ConditionalExpressionNode> node = ConditionalExpressionNode::parseConditionalExpression(lex);
    unique_ptr<RelationalExpressionNode> not_node = getRelationalExpression("x != 0)");
    unique_ptr<NotNode> lhs = make_unique<NotNode>(move(not_node));
    unique_ptr<RelationalExpressionNode> lhs_inner = getRelationalExpression("y == 0)");
    unique_ptr<RelationalExpressionNode> rhs_inner = getRelationalExpression("z == (5 + x))");
    unique_ptr<OrNode> rhs = make_unique<OrNode>(move(lhs_inner), move(rhs_inner));
    unique_ptr<AndNode> expected = make_unique<AndNode>(move(lhs), move(rhs));
    REQUIRE(expected->equals(move(node)));
}