#include "SP/Node/ProcedureNode.h"

#include "../MockUtilities.h"
#include "../TestUtilities.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ProcedureNode::equals") {
	ProcRef name_1 = "testName";
	ProcRef name_2 = "testName2";
	string stmt_1 = "print flag; call x; }";
	string stmt_2 = "cenX = 0; cenY = 0; }";
	unique_ptr<StatementListNode> stmt_lst = SP::TestUtilities::createStatementList(stmt_1, 2);
	shared_ptr<ProcedureNode> node = make_shared<ProcedureNode>(name_1, move(stmt_lst), 1, 2);

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
		REQUIRE(node->equals(other));
	}

	SECTION("Different Name Test") {
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_2, move(stmt_lst_2), 1, 2);
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different StmtLst Test") {
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 2);
		shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Start Line Number Test") {
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 2, 2);
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different End Line Number Test") {
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		shared_ptr<ProcedureNode> other = make_shared<ProcedureNode>(name_1, move(stmt_lst_2), 1, 3);
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::ProcedureNode::parseProcedure") {
	SP::Lexer lex;
	StmtRef statement_count = 1;

	SECTION("Valid Token Test") {
		lex.initialize("procedure testName { count = 0; }");
		unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList("count = 0; }", 1);
		shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2), 1, 1);
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Keyword As Name Test") {
		lex.initialize("procedure procedure { count = 0; }");
		unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList("count = 0; }", 1);
		shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("procedure", move(stmt_lst_2), 1, 1);
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Valid Complex Token Test") {
		lex.initialize("procedure testName { while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}");
		unique_ptr<ProcedureNode> node = ProcedureNode::parseProcedure(lex, statement_count);
		unique_ptr<StatementListNode> stmt_lst_2 =
			SP::TestUtilities::createStatementList("while ((x != 0) && (y != 0)) { cenX = cenX + 1; call readPoint; }}", 1);
		shared_ptr<ProcedureNode> expected = make_shared<ProcedureNode>("testName", move(stmt_lst_2), 1, 3);
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 4);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Invalid Grammar Test") {
		lex.initialize("procedures testName { cenX = 0; }");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "procedures");
	}

	SECTION("Invalid Name Test") {
		lex.initialize("procedure 1testName { cenX = 0; }");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "testName");
	}

	SECTION("Missing Opening Brackets Token Test") {
		lex.initialize("procedure testName cenX = 0; }");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "cenX");
	}

	SECTION("Missing Closing Brackets Token Test") {
		lex.initialize("procedure testName { cenX = 0; ");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("0 StmtLst Test") {
		lex.initialize("procedure testName {    } ");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_FALSE(lex.nextToken());
	}

	SECTION("Wrong Brackets Type Test") {
		lex.initialize("procedure testName ( cenX = 0; ) ");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "(");
	}

	SECTION("Invalid Brackets Test") {
		lex.initialize("procedure testName { (cenX = 0;) } ");
		REQUIRE_THROWS_AS(ProcedureNode::parseProcedure(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "cenX");
	}
}

TEST_CASE("SP::Node::ProcedureNode::extract Test") {
	MockStorageUpdate pkb;

	SECTION("Single statement") {
		StmtRef statement_number = 1;
		unique_ptr<StatementListNode> statements = make_unique<StatementListNode>();
		statements->addStatementNode(make_unique<ReadNode>(statement_number, make_unique<VariableNode>("A")));
		ProcedureNode node = ProcedureNode("Procedure", move(statements), statement_number, statement_number);
		node.extract(pkb);
		REQUIRE_EQUALS(pkb.set_proc_arguments, vector<tuple<ProcRef, StmtRef, StmtRef>>({{"Procedure", 1, 1}}));
		REQUIRE_EQUALS(pkb.set_stmt_type_call_count, 1);
	}

	SECTION("Multiple statements") {
		StmtRef first_statement_number = 1;
		StmtRef second_statement_number = 2;
		StmtRef third_statement_number = 3;
		unique_ptr<StatementListNode> statements = make_unique<StatementListNode>();
		statements->addStatementNode(make_unique<CallNode>(first_statement_number, "Procedure"));
		statements->addStatementNode(make_unique<ReadNode>(second_statement_number, make_unique<VariableNode>("A")));
		statements->addStatementNode(make_unique<PrintNode>(third_statement_number, make_unique<VariableNode>("B")));
		ProcedureNode node = ProcedureNode("Procedure", move(statements), first_statement_number, third_statement_number);
		node.extract(pkb);
		REQUIRE_EQUALS(pkb.set_proc_arguments, vector<tuple<ProcRef, StmtRef, StmtRef>>({{"Procedure", 1, 3}}));
		REQUIRE_EQUALS(pkb.set_stmt_type_call_count, 3);
	}
}