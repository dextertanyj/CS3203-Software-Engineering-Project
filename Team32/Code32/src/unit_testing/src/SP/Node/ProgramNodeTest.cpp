#include "SP/Node/ProgramNode.h"
#include "../Node/MockUtilities.h"
#include "SP/SP.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ProgramNode::equals") {
    ProcRef name_1 = "testname";
    ProcRef name_2 = "testname2";
    string stmt_1 = "print flag; call x; }";
    string stmt_2 = "cenX = 0; cenY = 0; }";
    shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList(stmt_1, 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>(name_1, move(stmt_lst), 1, 2);
    node->addProcedureNode(move(procedure));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(node));
    }

    SECTION("Same Node Test") {
        shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_1, move(stmt_lst_2),1 ,2);
        other->addProcedureNode(move(procedure_2));
        REQUIRE(node->equals(other));
    }

    SECTION("Different Length Test") {
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_2, 2);
        unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_2, move(stmt_lst_2), 2, 3);
        node->addProcedureNode(move(procedure_2));
        shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different Name Test") {
        shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_2, move(stmt_lst_2), 1, 2);
        other->addProcedureNode(move(procedure_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different Node Test") {
        shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_2, 2);
        unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
        other->addProcedureNode(move(procedure_2));
        REQUIRE_FALSE(node->equals(other));
    }
}

TEST_CASE("SP::Node::ProgramNode::addProcedureNode") {
    SECTION("Add Test") {
        unique_ptr<ProgramNode> node = make_unique<ProgramNode>();
        REQUIRE_EQUALS(node->getProcedures().size(), 0);
        unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
        unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst), 1, 2);
        REQUIRE_EQUALS(node->getProcedures().size(), 0);
        node->addProcedureNode(move(procedure));
        REQUIRE_EQUALS(node->getProcedures().size(), 1);
    }
}

TEST_CASE("SP::Node::ProgramNode::parseProgram") {
    SP::Lexer lex;
    StmtRef statement_count = 1;

    SECTION("Valid Token Test") {
        lex.initialize("procedure A { read x; } procedure B { read y; }");
        unique_ptr<ProgramNode> node = ProgramNode::parseProgram(lex, statement_count);
        shared_ptr<ProgramNode> expected = make_shared<ProgramNode>();
        unique_ptr<StatementListNode> stmt_lst = createStatementList("read x; }", 1);
        unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("A", move(stmt_lst), 1, 1);
        expected->addProcedureNode(move(procedure));
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("read y; }", 2);
        unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("B", move(stmt_lst_2), 2, 2);
        expected->addProcedureNode(move(procedure_2));
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("0 procedure Test") {
        lex.initialize(" ");
        REQUIRE_THROWS_AS(ProgramNode::parseProgram(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

}
