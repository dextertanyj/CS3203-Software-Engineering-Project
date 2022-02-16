#include "SP/Lexer.h"
#include "SP/Node/VariableNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::VariableNode::equals") {
    unique_ptr<VariableNode> node = make_unique<VariableNode>("A");

    SECTION("Same Object Test") {
        REQUIRE(node->equals(move(node)));
    }

    SECTION("Same Variable Name Test") {
        unique_ptr<VariableNode> other = make_unique<VariableNode>("A");
        REQUIRE(node->equals(move(other)));
    }

    SECTION("Different Variable Name Test") {
        unique_ptr<VariableNode> other = make_unique<VariableNode>("B");
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Case-Sensitivity Test") {
        unique_ptr<VariableNode> other = make_unique<VariableNode>("a");
        REQUIRE_FALSE(node->equals(move(other)));
    }
}

TEST_CASE("SP::Node::VariableNode::parseVariable") {
    SP::Lexer lex;

    SECTION("Parse Valid Only Letters Name Token") {
        lex.initialize("test");
        unique_ptr<VariableNode> node = VariableNode::parseVariable(lex);
        unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test"));
        REQUIRE(node->equals(move(other)));
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Parse Valid Name Token") {
        lex.initialize("test123 t1est23");
        unique_ptr<VariableNode> node = VariableNode::parseVariable(lex);
        unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test123"));
        REQUIRE(node->equals(move(other)));
        REQUIRE_EQUALS(lex.peekToken(), "t1est23");
        node = VariableNode::parseVariable(lex);
        other = make_unique<VariableNode>(VariableNode("t1est23"));
        REQUIRE(node->equals(move(other)));
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Parse Invalid Name Token") {
        lex.initialize("123test");
        REQUIRE_THROWS_AS(VariableNode::parseVariable(lex), SP::ParseException);
        REQUIRE_EQUALS(lex.peekToken(), "test");
    }

    SECTION("Parse Valid Only Letters Name String") {
        unique_ptr<VariableNode> node = VariableNode::parseVariable("test");
        unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test"));
        REQUIRE(node->equals(move(other)));
    }

    SECTION("Parse Valid Name String") {
        unique_ptr<VariableNode> node = VariableNode::parseVariable("test123");
        unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test123"));
        REQUIRE(node->equals(move(other)));
        node = VariableNode::parseVariable("t1est23");
        other = make_unique<VariableNode>(VariableNode("t1est23"));
        REQUIRE(node->equals(move(other)));
    }

    SECTION("Parse Invalid Name String") {
        REQUIRE_THROWS_AS(VariableNode::parseVariable("123test"), SP::ParseException);
        REQUIRE_THROWS_AS(VariableNode::parseVariable("test_123"), SP::ParseException);
    }
}

TEST_CASE("VariableNode::extract Test") {
	VarRef name = "Name";
	VariableNode node = VariableNode(name);
	VarRef result = node.extract();
	REQUIRE_EQUALS(result, name);
}
