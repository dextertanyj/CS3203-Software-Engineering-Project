#include "SP/Node/CallNode.h"
#include "SP/Node/ReadNode.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ReadNode::equals") {
    shared_ptr<ReadNode> node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(move(node)));
    }

    SECTION("Same Node Test") {
        shared_ptr<ReadNode> other = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
        REQUIRE(node->equals(move(other)));
    }

    SECTION("Different Node Test") {
        shared_ptr<ReadNode> other = make_shared<ReadNode>(1, make_unique<VariableNode>("b"));
        REQUIRE_FALSE(node->equals(move(other)));
        node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
        other = make_shared<ReadNode>(2, make_unique<VariableNode>("a"));
        REQUIRE_FALSE(node->equals(move(other)));
        node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
        other = make_shared<ReadNode>(2, make_unique<VariableNode>("b"));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Case-Sensitivity Test") {
        unique_ptr<ReadNode> other = make_unique<ReadNode>(1, make_unique<VariableNode>("A"));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Different Node Type Test") {
        shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
        REQUIRE_FALSE(node->equals(move(other)));
    }
}


TEST_CASE("SP::Node::ReadNode::parseReadStatement") {
    SP::Lexer lex;
    StmtRef statement_count = 1;

    SECTION("Valid Token Test") {
        lex.initialize("x;");
        unique_ptr<ReadNode> node = ReadNode::parseReadStatement(lex, statement_count);
        shared_ptr<ReadNode> expected = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Missing Semicolon Test") {
        lex.initialize("x");
        REQUIRE_THROWS_AS(ReadNode::parseReadStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Invalid Variable Name Test") {
        lex.initialize("1x;");
        REQUIRE_THROWS_AS(ReadNode::parseReadStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "x");
    }

    SECTION("Missing Variable Test") {
        lex.initialize(" ");
        REQUIRE_THROWS_AS(ReadNode::parseReadStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }
}

TEST_CASE("ReadNode::extract Test") {
	PKB pkb;
	ReadNode node = ReadNode(1, make_unique<VariableNode>("A"));
	StmtRef result = node.extract(pkb);
	StmtInfo expected = {1, StmtType::Read};
	REQUIRE_EQUALS(result, 1);
}
