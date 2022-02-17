#include "SP/Node/ProcedureNode.h"
#include "../Node/MockUtilities.h"
#include "SP/SP.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ProcedureNode::equals") {
    ProcRef name_1 = "testName";
    ProcRef name_2 = "testName2";
    string stmt_1 = "print flag; call x; }";
    string stmt_2 = "cenX = 0; cenY = 0; }";
    unique_ptr<StatementListNode> stmt_lst = createStatementList(stmt_1, 2);
    shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>(name_1, move(stmt_lst), 1, 2);

    SECTION("Same Object Test") {
        REQUIRE(node->equals(node));
    }

    SECTION("Same Node Test") {
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
        REQUIRE(node->equals(other));
    }

    SECTION("Different Name Test") {
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_2, move(stmt_lst_2), 1, 2);
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different StmtLst Test") {
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_2, 2);
        shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different Start Line Number Test") {
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 2, 2);
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different End Line Number Test") {
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 1, 3);
        REQUIRE_FALSE(node->equals(other));
    }
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure") {
    SP::Lexer lex;
    int statement_count = 1;

    SECTION("Valid Token Test") {
        lex.initialize("procedure testName { count = 0; }");
        unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("count = 0; }", 1);
        shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2), 1, 1);
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("Valid Complex Token Test") {
        lex.initialize("procedure testName { while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}");
        unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
        unique_ptr<StatementListNode> stmt_lst_2 =
                createStatementList("while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}", 1);
        shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2), 1, 3);
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 4);
    }

    SECTION("Invalid Grammar Test") {
        lex.initialize("procedures testName { cenX = 0; }");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Invalid Name Test") {
        lex.initialize("procedure 1testName { cenX = 0; }");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Missing Opening Brackets Token Test") {
        lex.initialize("procedure testName cenX = 0; }");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Missing Closing Brackets Token Test") {
        lex.initialize("procedure testName { cenX = 0; ");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("0 StmtLst Test") {
        lex.initialize("procedure testName {    } ");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Wrong Brackets Type Test") {
        lex.initialize("procedure testName ( cenX = 0; ) ");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Wrong Brackets Type Test") {
        lex.initialize("procedure testName ( cenX = 0; ) ");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 1);
    }

    SECTION("Invalid Brackets Test") {
        lex.initialize("procedure testName { (cenX = 0;) } ");
        REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
    }
}
