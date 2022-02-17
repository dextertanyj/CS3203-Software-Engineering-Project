#include "../Node/MockUtilities.h"
#include "SP/Lexer.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/SP.h"
#include "catch_tools.h"
#include <memory>

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::StatementListNode::equals") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    statement_list_node->addStatementNode(move(print_node));

    SECTION("Same Object Test") {
        REQUIRE(statement_list_node->equals(statement_list_node));
    }

    SECTION("Same Node Test") {
        shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
        unique_ptr<PrintNode> print_node_2 = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
        other->addStatementNode(move(print_node_2));
        REQUIRE(statement_list_node->equals(other));
    }

    SECTION("Different Length Test") {
        unique_ptr<PrintNode> print_node_2 = make_unique<PrintNode>(2, make_unique<VariableNode>("b"));
        statement_list_node->addStatementNode(move(print_node_2));
        shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
        REQUIRE_FALSE(statement_list_node->equals(other));
    }

    SECTION("Different Nodes Test") {
        unique_ptr<CallNode> call_node = make_unique<CallNode>(2, "test");
        statement_list_node->addStatementNode(move(call_node));
        shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
        unique_ptr<PrintNode> print_node_2 = make_unique<PrintNode>(1, make_unique<VariableNode>("b"));
        unique_ptr<CallNode> call_node_2 = make_unique<CallNode>(2, "test2");
        other->addStatementNode(move(print_node_2));
        other->addStatementNode(move(call_node_2));
        REQUIRE_FALSE(statement_list_node->equals(other));
    }
}

TEST_CASE("SP::Node::StatementListNode::addStatementNode") {
    shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
    REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 0);

    SECTION("CallNode Test") {
        unique_ptr<CallNode> call_node = make_unique<CallNode>(1, "test");
        statement_list_node->addStatementNode(move(call_node));
        REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
        vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
        shared_ptr<CallNode> call_node_shared = make_shared<CallNode>(1, "test");
        REQUIRE(stmt_list[0]->equals(call_node_shared));
        REQUIRE_EQUALS(stmt_list.size(), 1);
    }

    SECTION("PrintNode Test") {
        unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
        statement_list_node->addStatementNode(move(print_node));
        REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
        vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
        shared_ptr<PrintNode> print_node_shared = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
        REQUIRE(stmt_list[0]->equals(print_node_shared));
        REQUIRE_EQUALS(stmt_list.size(), 1);
    }

    SECTION("ReadNode Test") {
        unique_ptr<ReadNode> read_node = make_unique<ReadNode>(1, make_unique<VariableNode>("x"));
        statement_list_node->addStatementNode(move(read_node));
        REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
        vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
        shared_ptr<ReadNode> read_node_shared = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(stmt_list[0]->equals(read_node_shared));
        REQUIRE_EQUALS(stmt_list.size(), 1);
    }

    SECTION("Multiple Nodes Test") {
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
        REQUIRE_EQUALS(stmt_list.size(), 2);
    }
}

TEST_CASE("SP::Node::StatementListNode::parseStatementList") {
    SP::Lexer lex;
    StmtRef statement_count = 1;
    SECTION("Valid Token Test") {
        lex.initialize("print flag; read x; }");
        unique_ptr<StatementListNode> statement_list_node = StatementListNode::parseStatementList(lex, statement_count);
        shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
        unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("flag"));
        unique_ptr<ReadNode> read_node = make_unique<ReadNode>(2, make_unique<VariableNode>("x"));
        other->addStatementNode(move(print_node));
        other->addStatementNode(move(read_node));
        REQUIRE(statement_list_node->equals(other));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "}");
    }

    SECTION("Different Statement Number Test") {
        lex.initialize("print flag; read x; }");
        unique_ptr<StatementListNode> statement_list_node = StatementListNode::parseStatementList(lex, statement_count);
        shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
        unique_ptr<PrintNode> print_node = make_unique<PrintNode>(1, make_unique<VariableNode>("flag"));
        unique_ptr<ReadNode> read_node = make_unique<ReadNode>(3, make_unique<VariableNode>("x"));
        other->addStatementNode(move(print_node));
        other->addStatementNode(move(read_node));
        REQUIRE_FALSE(statement_list_node->equals(other));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "}");
    }

    SECTION("Missing Token Test") {
        lex.initialize("print x read y; }");
        REQUIRE_THROWS_AS(StatementListNode::parseStatementList(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "read");
    }

    SECTION("Extra Token Test") {
        lex.initialize("print x ;; print y; }");
        REQUIRE_THROWS_AS(StatementListNode::parseStatementList(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "print");
    }

    SECTION("Invalid Keyword Test") {
        lex.initialize("prints x ; }");
        REQUIRE_THROWS_AS(StatementListNode::parseStatementList(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "x");
    }
}

TEST_CASE("StatementListNode::extract Test") {
	PKB pkb;
	StatementListNode node = *createStatementList("read x; print y; }", 1).release();
	vector<StmtRef> result = node.extract(pkb);
	vector<StmtRef> expected = vector<StmtRef>({1, 2});
	REQUIRE_EQUALS(expected, result);
}
