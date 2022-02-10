#include "SP/Node/ProcedureNode.h"
#include "../Node/MockUtilities.h"
#include "SP/SP.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ProcedureNode::equals Same Object Test") {
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::ProcedureNode::equals Same Node Test") {
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>("testName", move(stmt_lst_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::ProcedureNode::equals Different Name Test") {
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>("anotherName", move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProcedureNode::equals Different StmtLst Test") {
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>("testName", move(stmt_lst));
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("cenX = 0; cenY = 0; }", 2);
    shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>("anotherName", move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Valid Token Test") {
	SP::Lexer lex;
    lex.initialize("procedure testName { count = 0; }");
    int statement_count = 1;
    unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("count = 0; }", 1);
    shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2));
    REQUIRE(node->equals(expected));
    REQUIRE_EQUALS(statement_count, 2);
}


TEST_CASE("SP::Node::ProcedureNode::parseProcedure Valid Complex Token Test") {
	SP::Lexer lex;
    lex.initialize("procedure testName { while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}");
    int statement_count = 1;
    unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
    unique_ptr<StatementListNode> stmt_lst_2 =
            createStatementList("while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}", 1);
    shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2));
    REQUIRE(node->equals(expected));
    REQUIRE_EQUALS(statement_count, 4);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Invalid Grammar Test") {
    SP::Lexer lex;
    lex.initialize("procedures testName { cenX = 0; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Invalid Name Test") {
	SP::Lexer lex;
    lex.initialize("procedure 1testName { cenX = 0; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Missing Opening Brackets Token Test") {
	SP::Lexer lex;
    lex.initialize("procedure testName cenX = 0; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Missing Closing Brackets Token Test") {
	SP::Lexer lex;
    lex.initialize("procedure testName { cenX = 0; ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure 0 StmtLst Test") {
    SP::Lexer lex;
    lex.initialize("procedure testName {    } ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Wrong Brackets Type Test") {
    SP::Lexer lex;
    lex.initialize("procedure testName ( cenX = 0; ) ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure Invalid Brackets Test") {
    SP::Lexer lex;
    lex.initialize("procedure testName { (cenX = 0;) } ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}