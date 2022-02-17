#include "SP/SP.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "../Node/MockUtilities.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::IfNode::equals") {
    string stmt_1 = "print flag; call x; }";
    string stmt_2 = "cenX = 0; read x; }";
    string stmt_3 = "read y; x = 10; }";
    unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> if_stmt_lst = createStatementList(stmt_1, 2);
    unique_ptr<StatementListNode> else_stmt_lst = createStatementList(stmt_2, 4);
    shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr), move(if_stmt_lst), move(else_stmt_lst));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(node));
    }

    SECTION("Same Node Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList(stmt_1, 2);
        unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList(stmt_2, 4);
        shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                       move(if_stmt_lst_2), move(else_stmt_lst_2));
        REQUIRE(node->equals(other));
    }

    SECTION("Different StmtNo Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList(stmt_1, 3);
        unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList(stmt_2, 5);
        shared_ptr<IfNode> other = make_shared<IfNode>(2, move(cond_expr_2),
                                                       move(if_stmt_lst_2), move(else_stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different CondExpr Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList(stmt_1, 2);
        unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList(stmt_2, 4);
        shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                       move(if_stmt_lst_2), move(else_stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different IfStmtLst Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList(stmt_3, 2);
        unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList(stmt_2, 4);
        shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                       move(if_stmt_lst_2), move(else_stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different ElseStmtLst Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst_2 = createStatementList(stmt_1, 2);
        unique_ptr<StatementListNode> else_stmt_lst_2 = createStatementList(stmt_3, 4);
        shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2),
                                                       move(if_stmt_lst_2), move(else_stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different Node Type Test") {
        shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
        REQUIRE_FALSE(node->equals(move(other)));
    }
}

TEST_CASE("SP::Node::IfNode::parseIfStatement") {
    StmtRef statement_count = 1;
    SP::Lexer lex;

    SECTION("Valid Token Test") {
        lex.initialize("(( x > 0 )) then { read y; } else { x = 10; }");
        unique_ptr<IfNode> node = IfNode::parseIfStatement(lex, statement_count);
        unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", ">", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst = createStatementList("read y; }", 2);
        unique_ptr<StatementListNode> else_stmt_lst = createStatementList("x = 10; }", 3);
        shared_ptr<IfNode> expected = make_shared<IfNode>(1, move(cond_expr),
                                                          move(if_stmt_lst), move(else_stmt_lst));
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 4);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Missing Condition Test") {
        lex.initialize("(  ) then { read y; } else { x = 10; }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), Common::ExpressionProcessor::ExpressionProcessorException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "then");
    }

    SECTION("Missing Condition Open Bracket Test") {
        lex.initialize("x == 10) then { read y; } else { x = 10; }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "x");
    }

    SECTION("Missing Condition Close Bracket Test") {
        lex.initialize("(x == 10 then { read y; } else { x = 10; }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "then");
    }

    SECTION("Missing Then Keyword Test") {
        lex.initialize("(x == 10) else { x = 10; }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "else");
    }

    SECTION("Empty Then Block Test") {
        lex.initialize("(x == 10) then {  } else { x = 10; }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "else");
    }

    SECTION("Missing Else Keyword Test") {
        lex.initialize("( x > 0 ) then { read y; } ");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Empty Else Block Test") {
        lex.initialize("(x == 10) then { read y; } else {  }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Missing Brackets Test") {
        lex.initialize("( x > 0 ) then  read y; } else { x = 10; }");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "read");
    }

    SECTION("Wrong Brackets Type Test") {
        lex.initialize("( x > 0 ) then { read y; } else ( x = 10; )");
        REQUIRE_THROWS_AS(IfNode::parseIfStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "(");
    }
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