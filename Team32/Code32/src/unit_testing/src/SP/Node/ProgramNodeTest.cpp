#include "SP/Node/ProgramNode.h"

#include "../MockUtilities.h"
#include "../TestUtilities.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintReadNode.tpp"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::ProgramNode::equals") {
	ProcRef name_1 = "testname";
	ProcRef name_2 = "testname2";
	string stmt_1 = "print flag; call x; }";
	string stmt_2 = "cenX = 0; cenY = 0; }";
	shared_ptr<ProgramNode> node = make_shared<ProgramNode>();
	unique_ptr<StatementListNode> stmt_lst = SP::TestUtilities::createStatementList(stmt_1, 2);
	unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>(name_1, move(stmt_lst), 1, 2);
	node->addProcedureNode(move(procedure));

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
		other->addProcedureNode(move(procedure_2));
		REQUIRE(node->equals(other));
	}

	SECTION("Different Length Test") {
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 2);
		unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_2, move(stmt_lst_2), 2, 3);
		node->addProcedureNode(move(procedure_2));
		shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Name Test") {
		shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_1, 2);
		unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_2, move(stmt_lst_2), 1, 2);
		other->addProcedureNode(move(procedure_2));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Node Test") {
		shared_ptr<ProgramNode> other = make_shared<ProgramNode>();
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList(stmt_2, 2);
		unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>(name_1, move(stmt_lst_2), 1, 2);
		other->addProcedureNode(move(procedure_2));
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::ProgramNode::addProcedureNode") {
	SECTION("Add Test") {
		unique_ptr<ProgramNode> node = make_unique<ProgramNode>();
		REQUIRE_EQUALS(node->getProcedures().size(), 0);
		unique_ptr<StatementListNode> stmt_lst = SP::TestUtilities::createStatementList("print flag; call x; }", 2);
		unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("testName", move(stmt_lst), 1, 2);
		REQUIRE_EQUALS(node->getProcedures().size(), 0);
		node->addProcedureNode(move(procedure));
		REQUIRE_EQUALS(node->getProcedures().size(), 1);
	}
}

TEST_CASE("SP::Node::ProgramNode::parse") {
	SP::Lexer lex;
	StmtRef statement_count = 1;

	SECTION("Valid Token Test") {
		lex.initialize("procedure A { read x; } procedure B { read y; }");
		unique_ptr<ProgramNode> node = ProgramNode::parse(lex, statement_count);
		shared_ptr<ProgramNode> expected = make_shared<ProgramNode>();
		unique_ptr<StatementListNode> stmt_lst = SP::TestUtilities::createStatementList("read x; }", 1);
		unique_ptr<ProcedureNode> procedure = make_unique<ProcedureNode>("A", move(stmt_lst), 1, 1);
		expected->addProcedureNode(move(procedure));
		unique_ptr<StatementListNode> stmt_lst_2 = SP::TestUtilities::createStatementList("read y; }", 2);
		unique_ptr<ProcedureNode> procedure_2 = make_unique<ProcedureNode>("B", move(stmt_lst_2), 2, 2);
		expected->addProcedureNode(move(procedure_2));
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 3);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("0 procedure Test") {
		lex.initialize(" ");
		REQUIRE_THROWS_AS(ProgramNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}
}

TEST_CASE("SP::Node::ProgramNode::extract Test") {
	MockStorageUpdate pkb;

	SECTION("Single procedure") {
		ProgramNode node = ProgramNode();
		StmtRef statement_number = 1;
		unique_ptr<StatementListNode> first_procedure = make_unique<StatementListNode>();
		first_procedure->addStatementNode(make_unique<PrintReadNode<StmtType::Read>>(statement_number, make_unique<VariableNode>("A")));
		node.addProcedureNode(make_unique<ProcedureNode>("First", move(first_procedure), statement_number, statement_number));
		node.extract(pkb);
		REQUIRE_EQUALS(pkb.set_modifies_call_count, 1);
		REQUIRE_EQUALS(pkb.set_modifies_arguments, vector<tuple<StmtRef, VarRef>>({{statement_number, "A"}}));
	}

	SECTION("Multiple procedures") {
		ProgramNode node = ProgramNode();
		StmtRef first_statement_number = 1;
		unique_ptr<StatementListNode> first_procedure = make_unique<StatementListNode>();
		first_procedure->addStatementNode(
			make_unique<PrintReadNode<StmtType::Read>>(first_statement_number, make_unique<VariableNode>("A")));
		node.addProcedureNode(make_unique<ProcedureNode>("First", move(first_procedure), first_statement_number, first_statement_number));
		StmtRef second_statement_number = 2;
		unique_ptr<StatementListNode> second_procedure = make_unique<StatementListNode>();
		second_procedure->addStatementNode(
			make_unique<PrintReadNode<StmtType::Read>>(second_statement_number, make_unique<VariableNode>("B")));
		node.addProcedureNode(
			make_unique<ProcedureNode>("Second", move(second_procedure), second_statement_number, second_statement_number));
		node.extract(pkb);
		REQUIRE_EQUALS(pkb.set_modifies_call_count, 2);
		REQUIRE_EQUALS(pkb.set_modifies_arguments,
		               vector<tuple<StmtRef, VarRef>>({{first_statement_number, "A"}, {second_statement_number, "B"}}));
	}
}
