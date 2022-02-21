#include <memory>

#include "../Node/MockUtilities.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::WhileNode::equals") {
    string stmt_1 = "print flag; call x; }";
    string stmt_2 = "cenX = 0; read x; }";
    unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
    unique_ptr<StatementListNode> stmt_lst = createStatementList(stmt_1, 2);
    shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));

    SECTION("Same Object Test") {
        REQUIRE(node->equals(node));
    }

    SECTION("Same Node Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
        REQUIRE(node->equals(other));
    }

    SECTION("Different StmtNo Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<WhileNode> other = make_shared<WhileNode>(6, move(cond_expr_2), move(stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different CondExpr Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_1, 2);
        shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different StmtLst Test") {
        unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst_2 = createStatementList(stmt_2, 2);
        shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
        REQUIRE_FALSE(node->equals(other));
    }

    SECTION("Different Node Type Test") {
        shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
        REQUIRE_FALSE(node->equals(move(other)));
    }
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement") {
    SP::Lexer lex;
    StmtRef statement_count = 1;

    SECTION("Valid Token Test") {
        lex.initialize("(x == 0) { count = count + 1; call readPoint; }");
        unique_ptr<WhileNode> node = WhileNode::parseWhileStatement(lex, statement_count);
        unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst = createStatementList("count = count + 1; call readPoint; }", 2);
        shared_ptr<WhileNode> expected = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 4);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Missing Condition Token Test") {
        lex.initialize("( ) { count = count + 1; call readPoint; }");
        REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), Common::ExpressionProcessor::ExpressionProcessorException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "{");
    }

    SECTION("Missing Condition Open Brackets Test") {
        lex.initialize("x == 0) { count = count + 1; call readPoint; }");
        REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "x");
    }

    SECTION("Missing Condition Close Brackets Test") {
        lex.initialize("(x == 0 { count = count + 1; call readPoint; }");
        REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "{");
    }

    SECTION("Missing Branch Token Test") {
        lex.initialize("(x == 0) { }");
        REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Missing Opening Brackets Token Test") {
        lex.initialize("(x == 0)  count = count + 1; call readPoint; }");
        REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "count");
    }

    SECTION("Closing Opening Brackets Token Test") {
        lex.initialize("(x == 0) { count = count + 1; call readPoint; ");
        REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 4);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }
}

TEST_CASE("SP::Node::WhileNode::extract Test") {
	PKB::Storage pkb;

	SECTION("Single enclosed statement") {
		StmtRef statement_number = 2;
		StmtRef innner_statement = 3;
		unique_ptr<ExpressionNode> condition = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "<", "0"})));
		unique_ptr<StatementListNode> body = make_unique<StatementListNode>();
		body->addStatementNode(make_unique<CallNode>(innner_statement, "Procedure"));
		WhileNode node = WhileNode(statement_number, std::move(condition), std::move(body));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);

		REQUIRE(pkb.checkParents(statement_number, innner_statement));
		REQUIRE_FALSE(pkb.checkFollows(statement_number, innner_statement));
	}

	SECTION("Multiple enclosed statements") {
		StmtRef statement_number = 2;
		StmtRef first_innner_statement = 3;
		StmtRef second_innner_statement = 5;
		StmtRef third_innner_statement = 8;
		unique_ptr<ExpressionNode> condition = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "<", "0", ")"})));
		unique_ptr<StatementListNode> body = make_unique<StatementListNode>();
		body->addStatementNode(make_unique<CallNode>(first_innner_statement, "Procedure"));
		body->addStatementNode(make_unique<ReadNode>(second_innner_statement, make_unique<VariableNode>("A")));
		body->addStatementNode(make_unique<PrintNode>(third_innner_statement, make_unique<VariableNode>("B")));
		WhileNode node = WhileNode(statement_number, std::move(condition), std::move(body));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);

		REQUIRE(pkb.checkParents(statement_number, first_innner_statement));
		REQUIRE(pkb.checkParents(statement_number, second_innner_statement));
		REQUIRE(pkb.checkParents(statement_number, third_innner_statement));
		REQUIRE_FALSE(pkb.checkParents(first_innner_statement, second_innner_statement));

		REQUIRE(pkb.checkFollows(first_innner_statement, second_innner_statement));
		REQUIRE(pkb.checkFollows(second_innner_statement, third_innner_statement));
		REQUIRE_FALSE(pkb.checkFollows(statement_number, first_innner_statement));
	}
}
