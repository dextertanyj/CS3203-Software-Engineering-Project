#include "SP/Node/StatementListNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Lexer.h"
#include "SP/SP.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::StatementListNode::addStatementNode CallNode Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 0);
    unique_ptr<CallNode> call_node = make_unique<CallNode>(1, "test");
    statement_list_node->addStatementNode(move(call_node));
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
    vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
    shared_ptr<CallNode> call_node_shared = make_shared<CallNode>(1, "test");
    REQUIRE(stmt_list[0]->equals(call_node_shared));
}

TEST_CASE("SP::Node::StatementListNode::addStatementNode PrintNode Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 0);
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    statement_list_node->addStatementNode(move(print_node));
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
    vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
    shared_ptr<PrintNode> print_node_shared = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(stmt_list[0]->equals(print_node_shared));
}

TEST_CASE("SP::Node::StatementListNode::addStatementNode ReadNode Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 0);
    unique_ptr<ReadNode> read_node = make_unique<ReadNode>(1, make_unique<VariableNode>("x"));
    statement_list_node->addStatementNode(move(read_node));
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
    vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
    shared_ptr<ReadNode> read_node_shared = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
    REQUIRE(stmt_list[0]->equals(read_node_shared));
}

TEST_CASE("SP::Node::StatementListNode::addStatementNode Multiple Nodes Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 0);
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    unique_ptr<CallNode> call_node = make_unique<CallNode>(2, "test");
    statement_list_node->addStatementNode(move(print_node));
    statement_list_node->addStatementNode(move(call_node));
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 2);
    vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
    shared_ptr<PrintNode> print_node_shared = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<CallNode> call_node_shared = make_shared<CallNode>(2, "test");
    REQUIRE(stmt_list[0]->equals(print_node_shared));
    REQUIRE(stmt_list[1]->equals(call_node_shared));
}

TEST_CASE("SP::Node::StatementListNode::equals Same Object Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    statement_list_node->addStatementNode(move(print_node));
    REQUIRE(statement_list_node->equals(statement_list_node));
}

TEST_CASE("SP::Node::StatementListNode::equals Same Node Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    statement_list_node->addStatementNode(move(print_node));
    shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node_2 = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    other->addStatementNode(move(print_node_2));
    REQUIRE(statement_list_node->equals(other));
}

TEST_CASE("SP::Node::StatementListNode::equals Different Length Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    unique_ptr<PrintNode> print_node_2 = make_unique<PrintNode>(2, make_unique<VariableNode>("b"));
    statement_list_node->addStatementNode(move(print_node));
    statement_list_node->addStatementNode(move(print_node_2));
    shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
    REQUIRE_FALSE(statement_list_node->equals(other));
}

TEST_CASE("SP::Node::StatementListNode::equals Different Nodes Test") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    unique_ptr<CallNode> call_node = make_unique<CallNode>(2, "test");
    statement_list_node->addStatementNode(move(print_node));
    statement_list_node->addStatementNode(move(call_node));
    shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node_2 = make_unique<PrintNode>(1, make_unique<VariableNode>("b"));
    unique_ptr<CallNode> call_node_2 = make_unique<CallNode>(2, "test2");
    other->addStatementNode(move(print_node_2));
    other->addStatementNode(move(call_node_2));
    REQUIRE_FALSE(statement_list_node->equals(other));
}

TEST_CASE("SP::Node::StatementListNode::parseStatementList Valid Token Test") {
    Lexer lex;
    lex.initialize("print flag; read x; }");
    int statement_count = 1;
    unique_ptr<StatementListNode> statement_list_node = StatementListNode::parseStatementList(lex, statement_count);
    shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("flag"));
    unique_ptr<ReadNode> read_node = make_unique<ReadNode>(2, make_unique<VariableNode>("x"));
    other->addStatementNode(move(print_node));
    other->addStatementNode(move(read_node));
    REQUIRE(statement_list_node->equals(other));
}

TEST_CASE("SP::Node::StatementListNode::parseStatementList Different Statement Number Test") {
    Lexer lex;
    lex.initialize("print flag; read x; }");
    int statement_count = 1;
    unique_ptr<StatementListNode> statement_list_node = StatementListNode::parseStatementList(lex, statement_count);
    shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("flag"));
    unique_ptr<ReadNode> read_node = make_unique<ReadNode>(3, make_unique<VariableNode>("x"));
    other->addStatementNode(move(print_node));
    other->addStatementNode(move(read_node));
    REQUIRE_FALSE(statement_list_node->equals(other));
}

TEST_CASE("SP::Node::StatementListNode::parseStatementList Missing Token Test") {
    Lexer lex;
    lex.initialize("print x read y; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(StatementListNode::parseStatementList(lex, statement_count), SP::TokenizationException);
}

TEST_CASE("SP::Node::StatementListNode::parseStatementList Extra Token Test") {
    Lexer lex;
    lex.initialize("print x ;; read y; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(StatementListNode::parseStatementList(lex, statement_count), SP::ParseException);
}

TEST_CASE("SP::Node::StatementListNode::parseStatementList Invalid Statement Test") {
    Lexer lex;
    lex.initialize("prints x ; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(StatementListNode::parseStatementList(lex, statement_count), SP::ParseException);
}
