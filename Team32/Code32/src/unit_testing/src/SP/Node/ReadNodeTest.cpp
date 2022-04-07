#include <tuple>
#include <vector>

#include "../MockUtilities.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/PrintReadNode.tpp"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::PrintReadNode<StmtType::Read>::equals") {
	shared_ptr<PrintReadNode<StmtType::Read>> node = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("a"));

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		shared_ptr<PrintReadNode<StmtType::Read>> other = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("a"));
		REQUIRE(node->equals(other));
	}

	SECTION("Different Node Test") {
		shared_ptr<PrintReadNode<StmtType::Read>> other = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("b"));
		REQUIRE_FALSE(node->equals(other));
		node = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("a"));
		other = make_shared<PrintReadNode<StmtType::Read>>(2, make_unique<VariableNode>("a"));
		REQUIRE_FALSE(node->equals(other));
		node = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("a"));
		other = make_shared<PrintReadNode<StmtType::Read>>(2, make_unique<VariableNode>("b"));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Case-Sensitivity Test") {
		shared_ptr<PrintReadNode<StmtType::Read>> other = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("A"));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Node Type Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::PrintReadNode<StmtType::Read>::parse") {
	SP::Lexer lex;
	StmtRef statement_count = 1;

	SECTION("Valid Token Test") {
		lex.initialize("x;");
		unique_ptr<PrintReadNode<StmtType::Read>> node = PrintReadNode<StmtType::Read>::parse(lex, statement_count);
		shared_ptr<PrintReadNode<StmtType::Read>> expected = make_shared<PrintReadNode<StmtType::Read>>(1, make_unique<VariableNode>("x"));
		REQUIRE(node->equals(move(expected)));
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Missing Semicolon Test") {
		lex.initialize("x");
		REQUIRE_THROWS_AS(PrintReadNode<StmtType::Read>::parse(lex, statement_count), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Invalid Variable Name Test") {
		lex.initialize("1x;");
		REQUIRE_THROWS_AS(PrintReadNode<StmtType::Read>::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "x");
	}

	SECTION("Missing Variable Test") {
		lex.initialize(" ");
		REQUIRE_THROWS_AS(PrintReadNode<StmtType::Read>::parse(lex, statement_count), SP::ParseException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}
}

TEST_CASE("SP::Node::PrintReadNode<StmtType::Read>::extract Test") {
	MockStorageUpdate pkb;
	StmtRef statement_number = 2;
	PrintReadNode<StmtType::Read> node = PrintReadNode<StmtType::Read>(statement_number, make_unique<VariableNode>("A"));
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, statement_number);
	REQUIRE_EQUALS(pkb.set_stmt_type_call_count, 1);
	REQUIRE_EQUALS(pkb.set_modifies_call_count, 1);
	REQUIRE_EQUALS(pkb.set_modifies_arguments, vector<tuple<StmtRef, VarRef>>({{statement_number, "A"}}));
}
