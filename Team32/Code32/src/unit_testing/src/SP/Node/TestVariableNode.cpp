#include "SP/Lexer.h"
#include "SP/Node/VariableNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::VariableNode::equals Same Object Test") {
	unique_ptr<VariableNode> node = make_unique<VariableNode>("A");
	REQUIRE(node->equals(move(node)));
}

TEST_CASE("SP::Node::VariableNode::equals Same Variable Name Test") {
	unique_ptr<VariableNode> node = make_unique<VariableNode>("A");
	unique_ptr<VariableNode> other = make_unique<VariableNode>("A");
	REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::equals Different Variable Name Test") {
	unique_ptr<VariableNode> node = make_unique<VariableNode>("A");
	unique_ptr<VariableNode> other = make_unique<VariableNode>("B");
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::equals Case-Sensitivity Test") {
	unique_ptr<VariableNode> node = make_unique<VariableNode>("A");
	unique_ptr<VariableNode> other = make_unique<VariableNode>("a");
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Only Letters Name Token") {
	SP::Lexer lex;
	lex.initialize("test");
	unique_ptr<VariableNode> node = VariableNode::parseVariable(lex);
	unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test"));
	REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Name Token") {
	SP::Lexer lex;
	lex.initialize("test123 t1est23");
	unique_ptr<VariableNode> node = VariableNode::parseVariable(lex);
	unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test123"));
	REQUIRE(node->equals(move(other)));
	node = VariableNode::parseVariable(lex);
	other = make_unique<VariableNode>(VariableNode("t1est23"));
	REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse invalid Name Token with digits first") {
	SP::Lexer lex;
	lex.initialize("123test");
	REQUIRE_THROWS_AS(VariableNode::parseVariable(lex), SP::ParseException);
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Only Letters Name String") {
	unique_ptr<VariableNode> node = VariableNode::parseVariable("test");
	unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test"));
	REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse Valid Name String") {
	unique_ptr<VariableNode> node = VariableNode::parseVariable("test123");
	unique_ptr<VariableNode> other = make_unique<VariableNode>(VariableNode("test123"));
	REQUIRE(node->equals(move(other)));
	node = VariableNode::parseVariable("t1est23");
	other = make_unique<VariableNode>(VariableNode("t1est23"));
	REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::VariableNode::parseVariable Parse invalid Name String with digits first") {
	REQUIRE_THROWS_AS(VariableNode::parseVariable("123test"), SP::ParseException);
}

TEST_CASE("VariableNode::extract Test") {
	VariableNode node = VariableNode("A");
	VarRef result = node.extract();
	VarRef expected = "A";
	REQUIRE_EQUALS(result, expected);
}
