#include "SP/Node/ProcedureNode.h"
#include "../Node/MockArithmeticNode.h"
#include "SP/SP.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::ProcedureNode::equals Same Object Test") {
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::ProcedureNode::equals Same Node Test") {
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>("testName", move(stmt_lst_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::ProcedureNode::equals Different Name Test") {
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>("anotherName", move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProcedureNode::equals Different StmtLst Test") {
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("cenX = 0; cenY = 0; }", 2);
    shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>("anotherName", move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Valid Token Test") {
    Lexer lex;
    lex.initialize("procedure testName { count = 0; }");
    int statement_count = 1;
    unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("count = 0; }", 1);
    shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2));
    REQUIRE(node->equals(expected));
}


TEST_CASE("SP::Node::ProcedureNode::parseProcedure Valid Complex Token Test") {
    Lexer lex;
    lex.initialize("procedure testName { while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}");
    int statement_count = 1;
    unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
    unique_ptr<StatementListNode> stmt_lst_2 =
            getStatementList("while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}", 1);
    shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2));
    REQUIRE(node->equals(expected));
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure invalid Name Test") {
    Lexer lex;
    lex.initialize("procedure 1testName { cenX = 0; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Missing Opening Brackets Token Test") {
    Lexer lex;
    lex.initialize("procedure testName { cenX = 0; ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Missing Closing Brackets Token Test") {
    Lexer lex;
    lex.initialize("procedure testName { cenX = 0; ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
}