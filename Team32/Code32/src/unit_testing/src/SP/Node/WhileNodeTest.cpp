#include "SP/Node/WhileNode.h"

#include <memory>

#include "../MockUtilities.h"
#include "../TestUtilities.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::WhileNode::equals") {
	string stmt_1 = "print flag; call x; }";
	string stmt_2 = "cenX = 0; read x; }";
	unique_ptr<ExpressionNode> cond_expr =
		make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
	unique_ptr<StatementListNode> stmt_lst = SP::TestUtilities::createStatementList(stmt_1, 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
		REQUIRE(node->equals(other));
	}

	SECTION("Different StmtNo Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<WhileNode> other = make_shared<WhileNode>(6, move(cond_expr_2), move(stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different CondExpr Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "==", "0", ")"})));
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different StmtLst Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 2);
		shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Node Type Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement") {
	SP::Lexer lex;
	StmtRef statement_count = 1;

	SECTION("Valid Token Test") {
		lex.initialize("(x == 0) { count = count + 1; call readPoint; }");
		unique_ptr<WhileNode> node = WhileNode::parseWhileStatement(lex, statement_count);
		unique_ptr<ExpressionNode> cond_expr =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "==", "0", ")"})));
		unique_ptr<StatementListNode> stmt_lst = SP::TestUtilities::createStatementList("count = count + 1; call readPoint; }", 2);
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
	MockStorageUpdate pkb;

	SECTION("Single enclosed statement") {
		StmtRef statement_number = 2;
		StmtRef inner_statement = 3;
		unique_ptr<ExpressionNode> condition =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "<", "0"})));
		unique_ptr<StatementListNode> body = make_unique<StatementListNode>();
		body->addStatementNode(make_unique<CallNode>(inner_statement, "Procedure"));
		WhileNode node = WhileNode(statement_number, std::move(condition), std::move(body));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);
		REQUIRE_EQUALS(pkb.set_parent_call_count, 1);
		REQUIRE_EQUALS(pkb.set_parent_arguments, vector<tuple<StmtRef, StmtRef>>{{statement_number, inner_statement}});
		REQUIRE_EQUALS(pkb.set_parent_arguments.size(), 1);
		REQUIRE_EQUALS(pkb.set_follows_call_count, 0);
		REQUIRE(pkb.set_follows_arguments.empty());

		// while control variables
		REQUIRE_EQUALS(pkb.set_while_control_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_while_control_set_arguments, vector<tuple<StmtRef, VarRefSet>>({{statement_number, {"x"}}}));

		// Next relationship
		REQUIRE_EQUALS(pkb.set_next_call_count, 2);
		REQUIRE_EQUALS(pkb.set_next_arguments,
		               vector<tuple<StmtRef, StmtRef>>({{statement_number, inner_statement}, {inner_statement, statement_number}}));
	}

	SECTION("Multiple enclosed statements") {
		StmtRef statement_number = 2;
		StmtRef first_inner_statement = 3;
		StmtRef second_inner_statement = 5;
		StmtRef third_inner_statement = 8;
		unique_ptr<ExpressionNode> condition =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "<", "y"})));
		unique_ptr<StatementListNode> body = make_unique<StatementListNode>();
		body->addStatementNode(make_unique<CallNode>(first_inner_statement, "Procedure"));
		body->addStatementNode(make_unique<ReadNode>(second_inner_statement, make_unique<VariableNode>("A")));
		body->addStatementNode(make_unique<PrintNode>(third_inner_statement, make_unique<VariableNode>("B")));
		WhileNode node = WhileNode(statement_number, std::move(condition), std::move(body));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);
		REQUIRE_EQUALS(pkb.set_parent_call_count, 3);
		REQUIRE_EQUALS(pkb.set_parent_arguments, vector<tuple<StmtRef, StmtRef>>{{statement_number, first_inner_statement},
		                                                                         {statement_number, second_inner_statement},
		                                                                         {statement_number, third_inner_statement}});
		REQUIRE_EQUALS(pkb.set_parent_arguments.size(), 3);
		REQUIRE_EQUALS(pkb.set_follows_call_count, 2);
		REQUIRE_EQUALS(pkb.set_follows_arguments, vector<tuple<StmtRef, StmtRef>>{{first_inner_statement, second_inner_statement},
		                                                                          {second_inner_statement, third_inner_statement}});
		REQUIRE_EQUALS(pkb.set_follows_arguments.size(), 2);

		// if control variables
		REQUIRE_EQUALS(pkb.set_while_control_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_while_control_set_arguments, vector<tuple<StmtRef, VarRefSet>>({{statement_number, {"x", "y"}}}));

		// Next relationship
		REQUIRE_EQUALS(pkb.set_next_call_count, 4);
		REQUIRE_EQUALS(pkb.set_next_arguments, vector<tuple<StmtRef, StmtRef>>({{first_inner_statement, second_inner_statement},
		                                                                           {second_inner_statement, third_inner_statement},
		                                                                           {statement_number, first_inner_statement},
		                                                                           {third_inner_statement, statement_number}}));
	}
}
