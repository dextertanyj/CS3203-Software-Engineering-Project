#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::PrintNode::equals") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(move(node)));
    }

    SECTION("Same Node Test") {
        shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
        REQUIRE(node->equals(move(other)));
    }

    SECTION("Different Node Test") {
        shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("b"));
        REQUIRE_FALSE(node->equals(move(other)));
        node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
        other = make_shared<PrintNode>(2, make_unique<VariableNode>("a"));
        REQUIRE_FALSE(node->equals(move(other)));
        node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
        other = make_shared<PrintNode>(2, make_unique<VariableNode>("b"));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Case-Sensitivity Test") {
        unique_ptr<PrintNode> other = make_unique<PrintNode>(1, make_unique<VariableNode>("A"));
        REQUIRE_FALSE(node->equals(move(other)));
    }

    SECTION("Different Node Type Test") {
        shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
        REQUIRE_FALSE(node->equals(move(other)));
    }
}

TEST_CASE("SP::Node::PrintNode::parsePrintStatement") {
    SP::Lexer lex;
    StmtRef statement_count = 1;

    SECTION("Valid Token Test") {
        lex.initialize("x;");
        unique_ptr<PrintNode> node = PrintNode::parsePrintStatement(lex, statement_count);
        shared_ptr<PrintNode> expected = make_shared<PrintNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Missing Semicolon Test") {
        lex.initialize("x");
        REQUIRE_THROWS_AS(PrintNode::parsePrintStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Invalid Variable Name Test") {
        lex.initialize("1x;");
        REQUIRE_THROWS_AS(PrintNode::parsePrintStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "x");
    }

    SECTION("Missing Variable Test") {
        lex.initialize(" ");
        REQUIRE_THROWS_AS(PrintNode::parsePrintStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }
}

TEST_CASE("SP::Node::PrintNode::extract Test") {
	Storage pkb;
	StmtRef statement_number = 3;
	PrintNode node = PrintNode(statement_number, make_unique<VariableNode>("A"));
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, statement_number);
	REQUIRE(pkb.checkUses(statement_number, "A"));
	REQUIRE_FALSE(pkb.checkModifies(statement_number, "A"));
}
