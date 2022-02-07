#include "SP/SP.h"
#include "SP/Node/ArithmeticExpressionNode.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"
#include "SP/Node/WhileNode.h"
#include "../Node/MockUtilities.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::StatementNode::parseStatement Assignment Valid Token Test") {
    Lexer lex;
    lex.initialize("x = 10 + 3;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    unique_ptr<VariableNode> assignee = make_unique<VariableNode>("x");
    unique_ptr<ArithmeticExpressionNode> expression =
            make_unique<ArithmeticExpressionNode>(createArithmeticExpression(vector<string>({"10", "+", "3", ";"})));
    shared_ptr<AssignmentNode> expected =
            make_shared<AssignmentNode>(1, move(assignee), move(expression));
    REQUIRE(node->equals(move(expected)));
}

TEST_CASE("SP::Node::StatementNode::parseStatement Read Valid Token Test") {
    Lexer lex;
    lex.initialize("read x;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    shared_ptr<ReadNode> expected = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
    REQUIRE(node->equals(move(expected)));
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::StatementNode::parseStatement Print Valid Token Test") {
    Lexer lex;
    lex.initialize("print x;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    shared_ptr<PrintNode> expected = make_shared<PrintNode>(1, make_unique<VariableNode>("x"));
    REQUIRE(node->equals(move(expected)));
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::StatementNode::parseStatement Call Valid Token Test") {
    Lexer lex;
    lex.initialize("call readPoint;");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    shared_ptr<CallNode> expected = make_shared<CallNode>(1, "readPoint");
    REQUIRE(node->equals(move(expected)));
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::StatementNode::parseStatement While Valid Token Test") {
    Lexer lex;
    lex.initialize("while (x == 0) { count = count + 1; call readPoint; }");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    unique_ptr<ConditionalExpressionNode> cond_expr = createRelationalExpression("x == 0)");
    unique_ptr<StatementListNode> stmt_lst = createStatementList("count = count + 1; call readPoint; }", 2);
    shared_ptr<WhileNode> expected = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    REQUIRE(node->equals(move(expected)));
    REQUIRE_EQUALS(statement_count, 4);
}

TEST_CASE("SP::Node::StatementNode::parseStatement If Valid Token Test") {
    Lexer lex;
    lex.initialize("if ( x > 0 ) then { read y; } else { x = 10; }");
    int statement_count = 1;
    unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
    unique_ptr<ConditionalExpressionNode> cond_expr = createRelationalExpression("x > 0)");
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("read y; }", 2);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("x = 10; }", 3);
    shared_ptr<IfNode> expected = make_shared<IfNode>(1, move(cond_expr),
                                                      move(if_stmt_lst), move(else_stmt_lst));
    REQUIRE(node->equals(expected));
    REQUIRE_EQUALS(statement_count, 4);
}

TEST_CASE("SP::Node::StatementNode::parseStatement Invalid Keyword Test") {
    Lexer lex;
    lex.initialize("for ( x > 0 ) { read y; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(StatementNode::parseStatement(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}