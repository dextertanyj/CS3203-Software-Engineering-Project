#include "SP/Node/IfNode.h"

#include "../MockUtilities.h"
#include "../TestUtilities.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintReadNode.tpp"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::IfNode::equals") {
	string stmt_1 = "print flag; call x; }";
	string stmt_2 = "cenX = 0; read x; }";
	string stmt_3 = "read y; x = 10; }";
	unique_ptr<ExpressionNode> cond_expr =
		make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
	unique_ptr<StatementListNode> if_stmt_lst = SP::TestUtilities::createStatementList(stmt_1, 2);
	unique_ptr<StatementListNode> else_stmt_lst = SP::TestUtilities::createStatementList(stmt_2, 4);
	shared_ptr<IfNode> node = make_shared<IfNode>(1, move(cond_expr), move(if_stmt_lst), move(else_stmt_lst));

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> if_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		unique_ptr<StatementListNode> else_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 4);
		shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2), move(if_stmt_lst_2), move(else_stmt_lst_2));
		REQUIRE(node->equals(other));
	}

	SECTION("Different StmtNo Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> if_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 3);
		unique_ptr<StatementListNode> else_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 5);
		shared_ptr<IfNode> other = make_shared<IfNode>(2, move(cond_expr_2), move(if_stmt_lst_2), move(else_stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different CondExpr Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "==", "0", ")"})));
		unique_ptr<StatementListNode> if_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		unique_ptr<StatementListNode> else_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 4);
		shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2), move(if_stmt_lst_2), move(else_stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different IfLst Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> if_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_3, 2);
		unique_ptr<StatementListNode> else_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 4);
		shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2), move(if_stmt_lst_2), move(else_stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different ElseStmtLst Test") {
		unique_ptr<ExpressionNode> cond_expr_2 =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "!=", "0", ")"})));
		unique_ptr<StatementListNode> if_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		unique_ptr<StatementListNode> else_stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_3, 4);
		shared_ptr<IfNode> other = make_shared<IfNode>(1, move(cond_expr_2), move(if_stmt_lst_2), move(else_stmt_lst_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Node Type Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::IfNode::parse") {
	StmtRef statement_count = 1;
	SP::Lexer lex;

	SECTION("Valid Token Test") {
		lex.initialize("( x > 0 ) then { read y; } else { x = 10; }");
		unique_ptr<IfNode> node = IfNode::parse(lex, statement_count);
		unique_ptr<ExpressionNode> cond_expr =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", ">", "0", ")"})));
		unique_ptr<StatementListNode> if_stmt_lst = SP::TestUtilities::createStatementList("read y; }", 2);
		unique_ptr<StatementListNode> else_stmt_lst = SP::TestUtilities::createStatementList("x = 10; }", 3);
		shared_ptr<IfNode> expected = make_shared<IfNode>(1, move(cond_expr), move(if_stmt_lst), move(else_stmt_lst));
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 4);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Missing Condition Test") {
		lex.initialize("(  ) then { read y; } else { x = 10; }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "then");
	}

	SECTION("Missing Condition Open Bracket Test") {
		lex.initialize("x == 10) then { read y; } else { x = 10; }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "x");
	}

	SECTION("Missing Condition Close Bracket Test") {
		lex.initialize("(x == 10 then { read y; } else { x = 10; }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "then");
	}

	SECTION("Missing Then Keyword Test") {
		lex.initialize("(x == 10) else { x = 10; }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "else");
	}

	SECTION("Empty Then Block Test") {
		lex.initialize("(x == 10) then {  } else { x = 10; }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "else");
	}

	SECTION("Missing Else Keyword Test") {
		lex.initialize("( x > 0 ) then { read y; } ");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 3);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Empty Else Block Test") {
		lex.initialize("(x == 10) then { read y; } else {  }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 3);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Missing Brackets Test") {
		lex.initialize("( x > 0 ) then  read y; } else { x = 10; }");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "read");
	}

	SECTION("Wrong Brackets Type Test") {
		lex.initialize("( x > 0 ) then { read y; } else ( x = 10; )");
		REQUIRE_THROWS_AS(IfNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 3);
		REQUIRE_EQUALS(lex.peekToken(), "(");
	}
}

TEST_CASE("SP::Node::IfNode::extract Test") {
	MockStorageUpdate pkb;

	SECTION("Single enclosed statement") {
		StmtRef statement_number = 2;
		StmtRef then_statement = 3;
		StmtRef else_statement = 8;
		unique_ptr<ExpressionNode> condition =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "<", "0"})));
		unique_ptr<StatementListNode> then_body = make_unique<StatementListNode>();
		then_body->addStatementNode(make_unique<CallNode>(then_statement, "Procedure"));
		unique_ptr<StatementListNode> else_body = make_unique<StatementListNode>();
		else_body->addStatementNode(make_unique<PrintReadNode<StmtType::Read>>(else_statement, make_unique<VariableNode>("A")));
		IfNode node = IfNode(statement_number, std::move(condition), std::move(then_body), std::move(else_body));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);

		// Parent relationship
		REQUIRE_EQUALS(pkb.set_parent_call_count, 2);
		REQUIRE_EQUALS(pkb.set_parent_arguments,
		               vector<tuple<StmtRef, StmtRef>>{{statement_number, then_statement}, {statement_number, else_statement}});
		REQUIRE_EQUALS(pkb.set_parent_arguments.size(), 2);

		// if control variables
		REQUIRE_EQUALS(pkb.set_if_control_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_if_control_set_arguments, vector<tuple<StmtRef, VarRefSet>>({{statement_number, {"x"}}}));

		// Next relationship
		REQUIRE_EQUALS(pkb.set_if_next_call_count, 1);
		REQUIRE_EQUALS(pkb.set_if_next_arguments,
		               vector<tuple<StmtRef, StmtRef, StmtRef>>({{statement_number, then_statement, else_statement}}));
		REQUIRE_EQUALS(pkb.set_if_exit_call_count, 1);
		REQUIRE_EQUALS(pkb.set_if_exit_arguments,
		               vector<tuple<StmtRef, StmtRef, StmtRef>>({{then_statement, else_statement, statement_number}}));
		REQUIRE(pkb.set_follows_arguments.empty());
	}

	SECTION("Multiple enclosed statements") {
		StmtRef statement_number = 2;
		StmtRef first_then_statement = 3;
		StmtRef second_then_statement = 5;
		StmtRef third_then_statement = 8;
		StmtRef first_else_statement = 10;
		StmtRef second_else_statement = 13;
		StmtRef third_else_statement = 16;
		unique_ptr<ExpressionNode> condition =
			make_unique<ExpressionNode>(SP::TestUtilities::createConditionalExpression(vector<string>({"x", "<", "y"})));
		unique_ptr<StatementListNode> then_body = make_unique<StatementListNode>();
		then_body->addStatementNode(make_unique<CallNode>(first_then_statement, "Procedure"));
		then_body->addStatementNode(make_unique<PrintReadNode<StmtType::Read>>(second_then_statement, make_unique<VariableNode>("A")));
		then_body->addStatementNode(make_unique<PrintReadNode<StmtType::Print>>(third_then_statement, make_unique<VariableNode>("B")));
		unique_ptr<StatementListNode> else_body = make_unique<StatementListNode>();
		else_body->addStatementNode(make_unique<CallNode>(first_else_statement, "Other"));
		else_body->addStatementNode(make_unique<PrintReadNode<StmtType::Read>>(second_else_statement, make_unique<VariableNode>("Y")));
		else_body->addStatementNode(make_unique<PrintReadNode<StmtType::Print>>(third_else_statement, make_unique<VariableNode>("Z")));
		IfNode node = IfNode(statement_number, std::move(condition), std::move(then_body), std::move(else_body));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);
		REQUIRE_EQUALS(pkb.set_parent_call_count, 6);
		REQUIRE_EQUALS(pkb.set_parent_arguments, vector<tuple<StmtRef, StmtRef>>{{statement_number, first_then_statement},
		                                                                         {statement_number, second_then_statement},
		                                                                         {statement_number, third_then_statement},
		                                                                         {statement_number, first_else_statement},
		                                                                         {statement_number, second_else_statement},
		                                                                         {statement_number, third_else_statement}});
		REQUIRE_EQUALS(pkb.set_parent_arguments.size(), 6);

		// if control variables
		REQUIRE_EQUALS(pkb.set_if_control_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_if_control_set_arguments, vector<tuple<StmtRef, VarRefSet>>({{statement_number, {"x", "y"}}}));

		// Next relationship
		REQUIRE_EQUALS(pkb.set_if_next_call_count, 1);
		REQUIRE_EQUALS(pkb.set_if_next_arguments,
		               vector<tuple<StmtRef, StmtRef, StmtRef>>({{statement_number, first_then_statement, first_else_statement}}));
		REQUIRE_EQUALS(pkb.set_if_exit_call_count, 1);
		REQUIRE_EQUALS(pkb.set_if_exit_arguments,
		               vector<tuple<StmtRef, StmtRef, StmtRef>>({{third_then_statement, third_else_statement, statement_number}}));
	}
}
