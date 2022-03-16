#include "SP/Node/CallNode.h"

#include "SP/Node/PrintNode.h"
#include "catch_tools.h"
#include "../MockUtilities.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::CallNode::equals") {
	shared_ptr<CallNode> node = make_shared<CallNode>(1, "test");

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
		REQUIRE(node->equals(other));
	}

	SECTION("Different Node Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "test2");
		REQUIRE_FALSE(node->equals(other));
		node = make_shared<CallNode>(1, "test");
		other = make_shared<CallNode>(2, "test");
		REQUIRE_FALSE(node->equals(other));
		node = make_shared<CallNode>(1, "test");
		other = make_shared<CallNode>(2, "test2");
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Case-Sensitivity Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "Test");
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Node Type Test") {
		shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::CallNode::parseCallStatement") {
	SP::Lexer lex;
	StmtRef statement_count = 1;

	SECTION("Valid Token Test") {
		lex.initialize("readPoint;");
		unique_ptr<CallNode> node = CallNode::parseCallStatement(lex, statement_count);
		shared_ptr<CallNode> expected = make_shared<CallNode>(1, "readPoint");
		REQUIRE(node->equals(move(expected)));
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Missing Semicolon Test") {
		lex.initialize("readPoint");
		REQUIRE_THROWS_AS(CallNode::parseCallStatement(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Invalid Procedure Name Test") {
		lex.initialize("1readPoint;");
		REQUIRE_THROWS_AS(CallNode::parseCallStatement(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "readPoint");
	}

	SECTION("Missing Procedure Test") {
		lex.initialize(" ");
		REQUIRE_THROWS_AS(CallNode::parseCallStatement(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}
}

TEST_CASE("SP::Node::CallNode::extract Test") {
	MockStorageUpdate pkb;

	StmtRef statement_number = 1;
	CallNode node = CallNode(1, "Procedure");
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, statement_number);
}
