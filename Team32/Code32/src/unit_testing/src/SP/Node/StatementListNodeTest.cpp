#include "SP/Node/StatementListNode.h"

#include <memory>

#include "../MockUtilities.h"
#include "SP/Lexer.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintReadNode.tpp"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::StatementListNode::equals") {
	shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
	unique_ptr<PrintReadNode<StmtType::Print>> print_node = make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("a"));
	statement_list_node->addStatementNode(move(print_node));

	SECTION("Same Object Test") { REQUIRE(statement_list_node->equals(statement_list_node)); }

	SECTION("Same Node Test") {
		shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
		unique_ptr<PrintReadNode<StmtType::Print>> print_node_2 =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("a"));
		other->addStatementNode(move(print_node_2));
		REQUIRE(statement_list_node->equals(other));
	}

	SECTION("Different Length Test") {
		unique_ptr<PrintReadNode<StmtType::Print>> print_node_2 =
			make_unique<PrintReadNode<StmtType::Print>>(2, make_unique<VariableNode>("b"));
		statement_list_node->addStatementNode(move(print_node_2));
		shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
		REQUIRE_FALSE(statement_list_node->equals(other));
	}

	SECTION("Different Nodes Test") {
		unique_ptr<CallNode> call_node = make_unique<CallNode>(2, "test");
		statement_list_node->addStatementNode(move(call_node));
		shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
		unique_ptr<PrintReadNode<StmtType::Print>> print_node_2 =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("b"));
		unique_ptr<CallNode> call_node_2 = make_unique<CallNode>(2, "test2");
		other->addStatementNode(move(print_node_2));
		other->addStatementNode(move(call_node_2));
		REQUIRE_FALSE(statement_list_node->equals(other));
	}
}

TEST_CASE("SP::Node::StatementListNode::addStatementNode") {
	shared_ptr<StatementListNode> statement_list_node = make_shared<StatementListNode>();
	REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 0);

	SECTION("CallNode Test") {
		unique_ptr<CallNode> call_node = make_unique<CallNode>(1, "test");
		statement_list_node->addStatementNode(move(call_node));
		REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
		vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
		shared_ptr<CallNode> call_node_shared = make_shared<CallNode>(1, "test");
		REQUIRE(stmt_list[0]->equals(call_node_shared));
		REQUIRE_EQUALS(stmt_list.size(), 1);
	}

	SECTION("PrintNode Test") {
		unique_ptr<PrintReadNode<StmtType::Print>> print_node =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("a"));
		statement_list_node->addStatementNode(move(print_node));
		REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
		vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
		shared_ptr<PrintReadNode<StmtType::Print>> print_node_shared =
			make_shared<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("a"));
		REQUIRE(stmt_list[0]->equals(print_node_shared));
		REQUIRE_EQUALS(stmt_list.size(), 1);
	}

	SECTION("ReadNode Test") {
		unique_ptr<PrintReadNode<StmtType::Read>> read_node = make_unique<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("x"));
		statement_list_node->addStatementNode(move(read_node));
		REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 1);
		vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
		shared_ptr<PrintReadNode<StmtType::Read>> read_node_shared =
			make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("x"));
		REQUIRE(stmt_list[0]->equals(read_node_shared));
		REQUIRE_EQUALS(stmt_list.size(), 1);
	}

	SECTION("Multiple Nodes Test") {
		unique_ptr<PrintReadNode<StmtType::Print>> print_node =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("a"));
		unique_ptr<CallNode> call_node = make_unique<CallNode>(2, "test");
		statement_list_node->addStatementNode(move(print_node));
		statement_list_node->addStatementNode(move(call_node));
		REQUIRE_EQUALS(statement_list_node->getStatementList().size(), 2);
		vector<shared_ptr<StatementNode>> stmt_list = statement_list_node->getStatementList();
		shared_ptr<PrintReadNode<StmtType::Print>> print_node_shared =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("a"));
		shared_ptr<CallNode> call_node_shared = make_shared<CallNode>(2, "test");
		REQUIRE(stmt_list[0]->equals(print_node_shared));
		REQUIRE(stmt_list[1]->equals(call_node_shared));
		REQUIRE_EQUALS(stmt_list.size(), 2);
	}
}

TEST_CASE("SP::Node::StatementListNode::parse") {
	SP::Lexer lex;
	StmtRef statement_count = 1;
	SECTION("Valid Token Test") {
		lex.initialize("print flag; read x; }");
		unique_ptr<StatementListNode> statement_list_node = StatementListNode::parse(lex, statement_count);
		shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
		unique_ptr<PrintReadNode<StmtType::Print>> print_node =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("flag"));
		unique_ptr<PrintReadNode<StmtType::Read>> read_node = make_unique<PrintReadNode<StmtType::Read>>(2, make_unique<VariableNode>("x"));
		other->addStatementNode(move(print_node));
		other->addStatementNode(move(read_node));
		REQUIRE(statement_list_node->equals(other));
		REQUIRE_EQUALS(statement_count, 3);
		REQUIRE_EQUALS(lex.peekToken(), "}");
	}

	SECTION("Different Statement Number Test") {
		lex.initialize("print flag; read x; }");
		unique_ptr<StatementListNode> statement_list_node = StatementListNode::parse(lex, statement_count);
		shared_ptr<StatementListNode> other = make_shared<StatementListNode>();
		unique_ptr<PrintReadNode<StmtType::Print>> print_node =
			make_unique<PrintReadNode<StmtType::Print>>(1, make_unique<VariableNode>("flag"));
		unique_ptr<PrintReadNode<StmtType::Read>> read_node = make_unique<PrintReadNode<StmtType::Read>>(3, make_unique<VariableNode>("x"));
		other->addStatementNode(move(print_node));
		other->addStatementNode(move(read_node));
		REQUIRE_FALSE(statement_list_node->equals(other));
		REQUIRE_EQUALS(statement_count, 3);
		REQUIRE_EQUALS(lex.peekToken(), "}");
	}

	SECTION("Missing Token Test") {
		lex.initialize("print x read y; }");
		REQUIRE_THROWS_AS(StatementListNode::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "read");
	}

	SECTION("Extra Token Test") {
		lex.initialize("print x ;; print y; }");
		REQUIRE_THROWS_AS(StatementListNode::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "print");
	}

	SECTION("Invalid Keyword Test") {
		lex.initialize("prints x ; }");
		REQUIRE_THROWS_AS(StatementListNode::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "x");
	}
}

TEST_CASE("SP::Node::StatementListNode::extract Test") {
	MockStorageUpdate pkb;

	SECTION("Single enclosed statement") {
		StmtRef statement_number = 2;
		StatementListNode node = StatementListNode();
		node.addStatementNode(make_unique<PrintReadNode<StmtType::Print>>(statement_number, make_unique<VariableNode>("A")));
		vector<StmtRef> result = node.extract(pkb);
		vector<StmtRef> expected = vector<StmtRef>({statement_number});
		REQUIRE_EQUALS(expected, result);

		REQUIRE_EQUALS(pkb.set_uses_call_count, 1);
		REQUIRE_EQUALS(pkb.set_uses_arguments, vector<tuple<StmtRef, VarRef>>({{statement_number, "A"}}));

		REQUIRE_EQUALS(pkb.set_next_call_count, 0);
	}

	SECTION("Multiple enclosed statements") {
		StmtRef first_statement = 2;
		StmtRef second_statement = 4;
		StmtRef third_statement = 6;
		StatementListNode node = StatementListNode();
		node.addStatementNode(make_unique<PrintReadNode<StmtType::Print>>(first_statement, make_unique<VariableNode>("A")));
		node.addStatementNode(make_unique<CallNode>(second_statement, "Procedure"));
		node.addStatementNode(make_unique<PrintReadNode<StmtType::Read>>(third_statement, make_unique<VariableNode>("B")));
		vector<StmtRef> result = node.extract(pkb);
		vector<StmtRef> expected = vector<StmtRef>({first_statement, second_statement, third_statement});
		REQUIRE_EQUALS(expected, result);
		REQUIRE_EQUALS(pkb.set_uses_call_count, 1);
		REQUIRE_EQUALS(pkb.set_uses_arguments, vector<tuple<StmtRef, VarRef>>({{first_statement, "A"}}));
		REQUIRE_EQUALS(pkb.set_uses_arguments.size(), 1);
		REQUIRE_EQUALS(pkb.set_modifies_call_count, 1);
		REQUIRE_EQUALS(pkb.set_modifies_arguments, vector<tuple<StmtRef, VarRef>>({{third_statement, "B"}}));
		REQUIRE_EQUALS(pkb.set_modifies_arguments.size(), 1);

		REQUIRE_EQUALS(pkb.set_next_call_count, 2);
		REQUIRE_EQUALS(pkb.set_next_arguments,
		               vector<tuple<StmtRef, StmtRef>>({{first_statement, second_statement}, {second_statement, third_statement}}));
	}
}
