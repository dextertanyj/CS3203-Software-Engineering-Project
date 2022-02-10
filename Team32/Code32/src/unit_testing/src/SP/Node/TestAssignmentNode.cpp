#include "../Node/MockUtilities.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/VariableNode.h"
#include "catch_tools.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::AssignmentNode::equals Same Object Test") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	REQUIRE(node->equals(move(node)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Same Node Test") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression_2 =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_2), move(expression_2));
	REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different StmtNo Node Test") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression_2 =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(2, move(assignee_2), move(expression_2));
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different Assignee Node Test") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("B");
	unique_ptr<ExpressionNode> expression_2 =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_2), move(expression_2));
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different Expression Node Test") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression_2 =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"B", ";"})));
	shared_ptr<AssignmentNode> other = make_shared<AssignmentNode>(1, move(assignee_2), move(expression_2));
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::equals Different Node Type Test") {
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("A");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"A", ";"})));
	shared_ptr<AssignmentNode> node = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Valid Token Test") {
	SP::Lexer lex;
	lex.initialize("= 0;");
	int statement_count = 1;
	unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"0", ";"})));
	shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	REQUIRE(node->equals(expected));
	REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Complex Valid Token Test") {
	SP::Lexer lex;
	lex.initialize("= (((x + 6) * (3 - (8))));");
	int statement_count = 1;
	unique_ptr<AssignmentNode> node = AssignmentNode::parseAssignmentStatement(lex, statement_count, "count");
	unique_ptr<VariableNode> assignee = make_unique<VariableNode>("count");
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(
                vector<string>({"(", "x", "+", "6", ")", "*", "(", "3", "-", "8", ")", ";"})));
	unique_ptr<ExpressionNode> expression =
		make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"x", "+", "6", ";"})));
	shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
	REQUIRE(node->equals(expected));
	REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Invalid Assignee Name Test") {
    SP::Lexer lex;
    lex.initialize("(x + 6);");
    int statement_count = 1;
    REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count_1"), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Invalid Factor Name Test") {
    SP::Lexer lex;
    lex.initialize("(1count + 6);");
    int statement_count = 1;
    REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count_1"), SP::ParseException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Missing Token Test") {
	SP::Lexer lex;
	lex.initialize("(x + 6);");
	int statement_count = 1;
	REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
	REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Missing Terminal Token Test") {
	SP::Lexer lex;
	lex.initialize("= (x + 6)");
	int statement_count = 1;
	REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
	REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Invalid Format Test") {
	SP::Lexer lex;
	lex.initialize("== (x + 6);");
	int statement_count = 1;
	REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
	REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("SP::Node::AssignmentNode::parseAssignmentStatement Incomplete Brackets Test") {
    SP::Lexer lex;
    lex.initialize("== ((x + 6;)");
    int statement_count = 1;
    REQUIRE_THROWS_AS(AssignmentNode::parseAssignmentStatement(lex, statement_count, "count"), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 1);
}

TEST_CASE("AssignmentNode::extract Test") {
	SP::Lexer lex;
	PKB pkb;
	lex.initialize("1;");
	unique_ptr<ExpressionNode> expression = ExpressionNode::parseExpression(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	AssignmentNode node = AssignmentNode(1, make_unique<VariableNode>("A"), std::move(expression));
	StmtInfo result = node.extract(pkb);
	StmtInfo expected = {1, StmtType::Assign};
	REQUIRE_EQUALS(result.reference, expected.reference);
	REQUIRE_EQUALS(result.type, expected.type);
}
