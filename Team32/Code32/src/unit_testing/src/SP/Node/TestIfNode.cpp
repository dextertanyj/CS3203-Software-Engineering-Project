#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "../Node/MockArithmeticNode.h"
#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "SP/SP.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::IfNode::equals Same Object Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("print flag; call x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("print flag; read x; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr), move(if_stmt_lst), move(else_stmt_lst));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::IfNode::equals Same Node Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
  move(if_stmt_lst), move(else_stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst_2 = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                  move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different StmtNo Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst_2 = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(2, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different CondExpr Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x == 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst_2 = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different IfStmtLst Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst_2 = getStatementList("print flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different ElseStmtLst Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst_2 = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = getStatementList("cenX = 0; print test; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different Node Type Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Valid Token Test") {
    Lexer lex;
    lex.initialize("( x > 0 ) then { read y; } else { x = 10; }");
    int statement_count = 1;
    unique_ptr<IfNode> node = IfNode::parseIfStatement(lex, statement_count);
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x > 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("read y; }", 2);
    unique_ptr<StatementListNode> else_stmt_lst = getStatementList("x = 10; }", 3);
    shared_ptr<IfNode> expected = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    REQUIRE(node->equals(expected));
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Condition Test") {
    Lexer lex;
    lex.initialize("(  ) then { read y; } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), ArithmeticProcessor::ArithmeticProcessorException);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Then Test") {
    Lexer lex;
    lex.initialize("(  ) then { } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), ArithmeticProcessor::ArithmeticProcessorException);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Else Test") {
    Lexer lex;
    lex.initialize("( x > 0 ) then { read y; } ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Brackets Test") {
    Lexer lex;
    lex.initialize("( x > 0 ) then  read y; } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
}