#include "SP/SP.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "../Node/MockUtilities.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::IfNode::equals Same Object Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("print flag; call x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("print flag; read x; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr), move(if_stmt_lst), move(else_stmt_lst));
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::IfNode::equals Same Node Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
  move(if_stmt_lst), move(else_stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                  move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different StmtNo Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(2, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different CondExpr Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different IfStmtLst Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList("print flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different ElseStmtLst Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList("cenX = 0; print test; }", 5);
    shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                   move(if_stmt_lst_2), move(else_stmt_lst_2));
    REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::IfNode::equals Different Node Type Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("call flag; read x; }", 3);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("cenX = 0; cenY = 0; }", 5);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Valid Token Test") {
	SP::Lexer lex;
    lex.initialize("( x > 0 ) then { read y; } else { x = 10; }");
    int statement_count = 1;
    unique_ptr<IfNode> node = IfNode::parseIfStatement(lex, statement_count);
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", ">", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList("read y; }", 2);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList("x = 10; }", 3);
    shared_ptr<IfNode> expected = make_shared<IfNode>(1, move(cond_expr),
                                                  move(if_stmt_lst), move(else_stmt_lst));
    REQUIRE(node->equals(expected));
    REQUIRE_EQUALS(statement_count, 4);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Condition Test") {
	SP::Lexer lex;
    lex.initialize("(  ) then { read y; } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), Common::ExpressionProcessor::ExpressionProcessorException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Condition Open Bracket Test") {
    SP::Lexer lex;
    lex.initialize("x == 10) then { read y; } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Condition Close Bracket Test") {
    SP::Lexer lex;
    lex.initialize("(x == 10 then { read y; } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Then Keyword Test") {
	SP::Lexer lex;
    lex.initialize("(x == 10) else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Empty Then Block Test") {
    SP::Lexer lex;
    lex.initialize("(x == 10) then {  } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Else Test") {
	SP::Lexer lex;
    lex.initialize("( x > 0 ) then { read y; } ");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 3);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Empty Else Block Test") {
    SP::Lexer lex;
    lex.initialize("(x == 10) then { read y; } else {  }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 3);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Missing Brackets Test") {
	SP::Lexer lex;
    lex.initialize("( x > 0 ) then  read y; } else { x = 10; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::IfNode::parseIfStatement Wrong Brackets Type Test") {
    SP::Lexer lex;
    lex.initialize("( x > 0 ) then  read y; } else ( x = 10; )");
    int statement_count = 1;
    REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("IfNode::extract Test") {
	PKB pkb;
	unique_ptr<ExpressionNode> condition = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "<", "y" ,")"})));
	unique_ptr<StatementListNode> if_clause = createStatementList("read x; print y; }", 2);
	unique_ptr<StatementListNode> then_clause = createStatementList("read x; print y; }", 4);
	IfNode node = IfNode(1, std::move(condition), std::move(if_clause), std::move(then_clause));
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, 1);
}