#include "SP/Node/AssignmentNode.h"

#include "../MockUtilities.h"
#include "../TestUtilities.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/VariableNode.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::AssignmentNode::equals") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(SP::TestUtilities::createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	unique_ptr<VariableNode> assignee_a = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression_a =
		make_unique<ExpressionNode>(SP::TestUtilities::createArithmeticExpression(vector<string>({"A", ";"})));

	SECTION("Same Object Test") { REQUIRE(node->equals(node)); }

	SECTION("Same Node Test") {
		shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_a), move(expression_a));
		REQUIRE(node->equals(other));
	}

	SECTION("Different StmtNo Node Test") {
		shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(2, move(assignee_a), move(expression_a));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Assignee Node Test") {
		unique_ptr<VariableNode> assignee_b = make_unique<VariableNode>("B");
		shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_b), move(expression_a));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Expression Node Test") {
		unique_ptr<ExpressionNode> expression_b =
			make_unique<ExpressionNode>(SP::TestUtilities::createArithmeticExpression(vector<string>({"B", ";"})));
		shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_a), move(expression_b));
		REQUIRE_FALSE(node->equals(other));
	}

	SECTION("Different Node Type Test") {
		shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
		REQUIRE_FALSE(node->equals(other));
	}
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement") {
	StmtRef statement_count = 1;
	SP::Lexer lex;

	SECTION("Valid Token Test") {
		lex.initialize("= (x + 6);");
		unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
		unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
		unique_ptr<ExpressionNode> expression =
			make_unique<ExpressionNode>(SP::TestUtilities::createArithmeticExpression(vector<string>({"(", "x", "+", "6", ")", ";"})));
		shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Complex Valid Token Test") {
		lex.initialize("= (((x + 6) * (3 - (8))));");
		unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
		unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
		unique_ptr<ExpressionNode> expression = make_unique<ExpressionNode>(
			SP::TestUtilities::createArithmeticExpression(vector<string>({"(", "x", "+", "6", ")", "*", "(", "3", "-", "8", ")", ";"})));
		shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
		REQUIRE(node->equals(expected));
		REQUIRE_EQUALS(statement_count, 2);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Missing Token Test") {
		lex.initialize("(x + 6);");
		REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "(");
	}

	SECTION("Missing Terminal Token Test") {
		lex.initialize("= (x + 6)");
		REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}

	SECTION("Invalid Format Test") {
		lex.initialize("== (x + 6);");
		REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "==");
	}

	SECTION("Incomplete Brackets Test") {
		lex.initialize("= ((x + 6);");
		REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"),
		                  Common::ExpressionProcessor::ExpressionProcessorException);
		REQUIRE_EQUALS(statement_count, 1);
		REQUIRE_EQUALS(lex.peekToken(), "");
	}
}

TEST_CASE("SP::Node::AssignmentNode::extract Test") {
	SP::Lexer lex;
	MockStorageUpdate pkb;

	SECTION("Single constant assignment") {
		lex.initialize("1;");
		StmtRef statement_number = 1;
		unique_ptr<ExpressionNode> expression =
			ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::ExpressionType::Arithmetic);
		AssignmentNode node = AssignmentNode(statement_number, make_unique<VariableNode>("A"), std::move(expression));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);
		REQUIRE_EQUALS(pkb.set_constant_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_constant_set_arguments, vector<unordered_set<ConstVal>>({{1}}));
		REQUIRE_EQUALS(pkb.set_modifies_call_count, 1);
		REQUIRE_EQUALS(pkb.set_modifies_arguments, vector<tuple<StmtRef, VarRef>>({{statement_number, "A"}}));
		REQUIRE_EQUALS(pkb.set_uses_call_count, 0);
		REQUIRE(pkb.set_uses_arguments.empty());
		vector<tuple<StmtRef, VarRef, Common::ExpressionProcessor::Expression>> res_set = pkb.set_assign_arguments;
		REQUIRE_EQUALS(pkb.set_assign_arguments,
		               vector<tuple<StmtRef, VarRef, Common::ExpressionProcessor::Expression>>(
						   {{statement_number, "A", SP::TestUtilities::createArithmeticExpression(vector<string>({"1"}))}}));
	}

	SECTION("Single variable assignment") {
		lex.initialize("B;");
		StmtRef statement_number = 2;
		unique_ptr<ExpressionNode> expression =
			ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::ExpressionType::Arithmetic);
		AssignmentNode node = AssignmentNode(statement_number, make_unique<VariableNode>("A"), std::move(expression));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);
		REQUIRE_EQUALS(pkb.set_modifies_call_count, 1);
		REQUIRE_EQUALS(pkb.set_modifies_arguments, vector<tuple<StmtRef, VarRef>>({{statement_number, "A"}}));
		REQUIRE_EQUALS(pkb.set_uses_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_uses_set_arguments,
		               vector<tuple<StmtRef, unordered_set<VarRef>>>({{statement_number, unordered_set<VarRef>{"B"}}}));
		REQUIRE_EQUALS(pkb.set_assign_arguments,
		               vector<tuple<StmtRef, VarRef, Common::ExpressionProcessor::Expression>>(
						   {{statement_number, "A", SP::TestUtilities::createArithmeticExpression(vector<string>({"B"}))}}));
	}

	SECTION("Multiple terminal assignment") {
		lex.initialize("1 + B * 2 / C;");
		StmtRef statement_number = 3;
		unique_ptr<ExpressionNode> expression =
			ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::ExpressionType::Arithmetic);
		AssignmentNode node = AssignmentNode(statement_number, make_unique<VariableNode>("A"), std::move(expression));
		StmtRef result = node.extract(pkb);
		REQUIRE_EQUALS(result, statement_number);
		REQUIRE_EQUALS(pkb.set_constant_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_constant_set_arguments, vector<unordered_set<ConstVal>>({{1, 2}}));
		REQUIRE_EQUALS(pkb.set_modifies_call_count, 1);
		REQUIRE_EQUALS(pkb.set_modifies_arguments, vector<tuple<StmtRef, VarRef>>({{statement_number, "A"}}));
		REQUIRE_EQUALS(pkb.set_uses_set_call_count, 1);
		REQUIRE_EQUALS(pkb.set_uses_set_arguments,
		               vector<tuple<StmtRef, unordered_set<VarRef>>>({{statement_number, unordered_set<VarRef>{"B", "C"}}}));
		REQUIRE_EQUALS(pkb.set_assign_arguments,
		               vector<tuple<StmtRef, VarRef, Common::ExpressionProcessor::Expression>>(
						   {{statement_number, "A",
		                     SP::TestUtilities::createArithmeticExpression(vector<string>({"1", "+", "B", "*", "2", "/", "C"}))}}));
	}
}
