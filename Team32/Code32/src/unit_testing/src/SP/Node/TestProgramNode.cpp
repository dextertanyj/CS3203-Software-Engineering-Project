#include "SP/Node/ProgramNode.h"
#include "../Node/MockUtilities.h"
#include "SP/SP.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::ProgramNode::addProcedureNode Add Test") {
    unique_ptr<ProgramNode> node = make_unique<ProgramNode>();
    REQUIRE_EQUALS(node->getProcedures().size(), 0);
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst));
    node->addProcedureNode(move(procedure));
    REQUIRE_EQUALS(node->getProcedures().size(), 1);
}

TEST_CASE("SP::Node::ProgramNode::equals Same Object Test") {
    shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst));
    node->addProcedureNode(move(procedure));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::ProgramNode::equals Same Node Test") {
    shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst));
    node->addProcedureNode(move(procedure));
    shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("testName", move(stmt_lst_2));
    other->addProcedureNode(move(procedure_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::ProgramNode::equals Different Length Test") {
    shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst));
    node->addProcedureNode(move(procedure));
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("testName2", move(stmt_lst_2));
    node->addProcedureNode(move(procedure_2));
    shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProgramNode::equals Different Name Test") {
    shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst));
    node->addProcedureNode(move(procedure));
    shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("testName2", move(stmt_lst_2));
    other->addProcedureNode(move(procedure_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProgramNode::equals Different Node Test") {
    shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst));
    node->addProcedureNode(move(procedure));
    shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("read flag; call y; }", 2);
    unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("testName", move(stmt_lst_2));
    other->addProcedureNode(move(procedure_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::ProgramNode::parseProgram Valid Token Test") {
    Lexer lex;
    lex.initialize("procedure A { read x; } procedure B { read y; }");
    int statement_count = 1;
    unique_ptr<ProgramNode> node = ProgramNode::parseProgram(lex, statement_count);
    shared_ptr<ProgramNode> expected = make_shared<ProgramNode>();
    unique_ptr<StatementListNode> stmt_lst = createStatementList("read x; }", 1);
    unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("A", move(stmt_lst));
    expected->addProcedureNode(move(procedure));
    unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("read y; }", 2);
    unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("B", move(stmt_lst_2));
    expected->addProcedureNode(move(procedure_2));
    REQUIRE(node->equals(expected));
    REQUIRE_EQUALS(statement_count, 3);
}