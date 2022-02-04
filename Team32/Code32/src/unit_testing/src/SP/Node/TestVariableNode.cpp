#include "SP/Node/VariableNode.h"
#include "SP/Lexer.h"
#include "SP/SP.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::VariableNode::equals Same Object Test") {
    unique_ptr<VariableNode> variable = make_unique<VariableNode>("A");
    REQUIRE(variable->equals(move(variable)));
}

TEST_CASE("SP::Node::VariableNode::equals Same Variable Name Test") {
    unique_ptr<VariableNode> variable = make_unique<VariableNode>("A");
    unique_ptr<VariableNode> other = make_unique<VariableNode>("A");
    REQUIRE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::equals Different Variable Name Test") {
    unique_ptr<VariableNode> variable = make_unique<VariableNode>("A");
    unique_ptr<VariableNode> other = make_unique<VariableNode>("B");
    REQUIRE_FALSE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::equals Case-Sensitivity Test") {
    unique_ptr<VariableNode> variable = make_unique<VariableNode>("A");
    unique_ptr<VariableNode> other = make_unique<VariableNode>("a");
    REQUIRE_FALSE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Only Letters Name Token") {
    Lexer lex;
    lex.initialize("test");
    unique_ptr<VariableNode> variable = VariableNode::parseVariable(lex);
    unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test"));
    REQUIRE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Name Token") {
    Lexer lex;
    lex.initialize("test123 t1est23");
    unique_ptr<VariableNode> variable = VariableNode::parseVariable(lex);
    unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test123"));
    REQUIRE(variable->equals(move(other)));
    variable = VariableNode::parseVariable(lex);
    other = make_unique<VariableNode>(VariableNode("t1est23"));
    REQUIRE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse invalid Name Token with digits first") {
    Lexer lex;
    lex.initialize("123test");
    REQUIRE_THROWS_AS(VariableNode::parseVariable(lex), SP::ParseException);
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Only Letters Name String") {
    unique_ptr<VariableNode> variable = VariableNode::parseVariable("test");
    unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test"));
    REQUIRE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Name String") {
    unique_ptr<VariableNode> variable = VariableNode::parseVariable("test123");
    unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test123"));
    REQUIRE(variable->equals(move(other)));
    variable = VariableNode::parseVariable("t1est23");
    other = make_unique<VariableNode>(VariableNode("t1est23"));
    REQUIRE(variable->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse invalid Name String with digits first") {
    REQUIRE_THROWS_AS(VariableNode::parseVariable("123test"), SP::ParseException);
}
