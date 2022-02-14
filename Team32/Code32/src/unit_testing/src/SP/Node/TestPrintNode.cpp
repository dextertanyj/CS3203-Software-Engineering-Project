#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::PrintNode::equals Same Object Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(node->equals(move(node)));
}

TEST_CASE("SP::Node::PrintNode::equals Same Node Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::PrintNode::equals Different Node Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("b"));
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    other = make_shared<PrintNode>(2, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    other = make_shared<PrintNode>(2, make_unique<VariableNode>("b"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::PrintNode::equals Case-Sensitivity Test") {
    unique_ptr<PrintNode> node = make_unique<PrintNode>(1, make_unique<VariableNode>("A"));
    unique_ptr<PrintNode> other = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::PrintNode::equals Different Node Type Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("PrintNode::extract Test") {
	PKB pkb;
	PrintNode node = PrintNode(1, make_unique<VariableNode>("A"));
	StmtInfo result = node.extract(pkb);
	StmtInfo expected = {1, StmtType::Print};
	REQUIRE_EQUALS(result.reference, expected.reference);
	REQUIRE_EQUALS(result.type, expected.type);
}

TEST_CASE("SP::Node::PrintNode::parsePrintStatement Valid Token Test") {
	SP::Lexer lex;
    lex.initialize("x;");
    int statement_count = 1;
    unique_ptr<PrintNode> node = PrintNode::parsePrintStatement(lex, statement_count);
    shared_ptr<PrintNode> expected = make_shared<PrintNode>(1, make_unique<VariableNode>("x"));
    REQUIRE(node->equals(move(expected)));
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::ReadNode::parsePrintStatement Missing Semicolon Test") {
    SP::Lexer lex;
    lex.initialize("x");
    int statement_count = 1;
    REQUIRE_THROWS_AS(PrintNode::parsePrintStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::ReadNode::parsePrintStatement Missing Variable Test") {
    SP::Lexer lex;
    lex.initialize(" ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(PrintNode::parsePrintStatement(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}