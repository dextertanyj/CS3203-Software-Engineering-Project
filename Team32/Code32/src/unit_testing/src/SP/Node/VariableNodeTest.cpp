#include "SP/Node/VariableNode.h"

#include "SP/Lexer.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::VariableNode::equals") {
	shared_ptr<VariableNode> node = make_shared<VariableNode>("A");

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Variable Name Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("A");
		REQUIRE(node->equals(other));
	}

	SECTION("Different Variable Name Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("B");
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Case-Sensitivity Test") {
		shared_ptr<VariableNode> other = make_shared<VariableNode>("a");
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::VariableNode::parseVariable") {
	SP::Lexer lex;

	SECTION("Parse Valid Only Letters Name Token") {
		lex.initialize("test");
		shared_ptr<VariableNode> node = VariableNode::parseVariable(lex);
		shared_ptr<VariableNode> other = make_shared<VariableNode>(VariableNode("test"));
		REQUIRE(node->equals(other));
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Parse Valid Name Token") {
		lex.initialize("test123 t1est23");
		shared_ptr<VariableNode> node = VariableNode::parseVariable(lex);
		shared_ptr<VariableNode> other = make_shared<VariableNode>(VariableNode("test123"));
		REQUIRE(node->equals(other));
		REQUIRE_EQUALS(lex.peekToken(), "t1est23");
		node = VariableNode::parseVariable(lex);
		other = make_shared<VariableNode>(VariableNode("t1est23"));
		REQUIRE(node->equals(other));
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Parse Invalid Name Token") {
		lex.initialize("123test");
		REQUIRE_THROWS_AS(VariableNode::parseVariable(lex), SP::ParseException);
		REQUIRE_EQUALS(lex.peekToken(), "test");
	}

	SECTION("Parse Valid Only Letters Name String") {
		shared_ptr<VariableNode> node = VariableNode::parseVariable("test");
		shared_ptr<VariableNode> other = make_shared<VariableNode>(VariableNode("test"));
		REQUIRE(node->equals(other));
	}

	SECTION("Parse Valid Name String") {
		shared_ptr<VariableNode> node = VariableNode::parseVariable("test123");
		shared_ptr<VariableNode> other = make_shared<VariableNode>(VariableNode("test123"));
		REQUIRE(node->equals(other));
		node = VariableNode::parseVariable("t1est23");
		other = make_shared<VariableNode>(VariableNode("t1est23"));
		REQUIRE(node->equals(other));
	}

	SECTION("Parse Invalid Name String") {
		REQUIRE_THROWS_AS(VariableNode::parseVariable("123test"), SP::ParseException);
		REQUIRE_THROWS_AS(VariableNode::parseVariable("test_123"), SP::ParseException);
	}
}

TEST_CASE("SP::Node::VariableNode::extract Test") {
	VarRef name = "Name";
	VariableNode node = VariableNode(name);
	VarRef result = node.extract();
	REQUIRE_EQUALS(result, name);
}
