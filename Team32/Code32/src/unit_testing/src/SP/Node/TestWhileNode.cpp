#include "SP/Node/CallNode.h"
#include "SP/Node/WhileNode.h"
#include "../Node/MockArithmeticNode.h"
#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "SP/SP.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::WhileNode::equals Same Object Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::WhileNode::equals Same Node Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different StmtNo Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> other = make_shared<WhileNode>(6, move(cond_expr_2), move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different CondExpr Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x == 0)");
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different StmtLst Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    unique_ptr<ConditionalExpressionNode> cond_expr_2 = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst_2 = getStatementList("cenX = 0; cenY = 0; }", 2);
    shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different Node Type Test") {
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x != 0)");
    unique_ptr<StatementListNode> stmt_lst = getStatementList("print flag; call x; }", 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Valid Token Test") {
    Lexer lex;
    lex.initialize("(x == 0) { count = count + 1; call readPoint; }");
    int statement_count = 1;
    unique_ptr<WhileNode> node = WhileNode::parseWhileStatement(lex, statement_count);
    unique_ptr<ConditionalExpressionNode> cond_expr = getRelationalExpression("x == 0)");
    unique_ptr<StatementListNode> if_stmt_lst = getStatementList("count = count + 1; call readPoint; }", 2);
    shared_ptr<WhileNode> expected = make_shared<WhileNode>(1, move(cond_expr), move(if_stmt_lst));
    REQUIRE(node->equals(expected));
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Condition Token Test") {
    Lexer lex;
    lex.initialize("( ) { count = count + 1; call readPoint; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), ArithmeticProcessor::ArithmeticProcessorException);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Branch Token Test") {
    Lexer lex;
    lex.initialize("(x == 0) { }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::ParseException);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Brackets Token Test") {
    Lexer lex;
    lex.initialize("(x == 0)  count = count + 1; call readPoint; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
}