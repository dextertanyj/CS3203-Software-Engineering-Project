#include <memory>

#include "../Node/MockUtilities.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::WhileNode::equals Same Object Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::WhileNode::equals Same Node Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
	REQUIRE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different StmtNo Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> other = make_shared<WhileNode>(6, move(cond_expr_2), move(stmt_lst_2));
	REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different CondExpr Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
	REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different StmtLst Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	unique_ptr<ExpressionNode> cond_expr_2 = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst_2 = createStatementList("cenX = 0; cenY = 0; }", 2);
	shared_ptr<WhileNode> other = make_shared<WhileNode>(1, move(cond_expr_2), move(stmt_lst_2));
	REQUIRE_FALSE(node->equals(other));
}

TEST_CASE("SP::Node::WhileNode::equals Different Node Type Test") {
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "!=", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("print flag; call x; }", 2);
	shared_ptr<WhileNode> node = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
	REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Valid Token Test") {
	SP::Lexer lex;
	lex.initialize("(x == 0) { count = count + 1; call readPoint; }");
	int statement_count = 1;
	unique_ptr<WhileNode> node = WhileNode::parseWhileStatement(lex, statement_count);
	unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
	unique_ptr<StatementListNode> stmt_lst = createStatementList("count = count + 1; call readPoint; }", 2);
	shared_ptr<WhileNode> expected = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
	REQUIRE(node->equals(expected));
	REQUIRE_EQUALS(statement_count, 4);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Condition Token Test") {
	SP::Lexer lex;
	lex.initialize("( ) { count = count + 1; call readPoint; }");
	int statement_count = 1;
	REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), Common::ExpressionProcessor::ExpressionProcessorException);
	REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Condition Open Brackets Test") {
    SP::Lexer lex;
    lex.initialize("x == 0) { count = count + 1; call readPoint; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Condition Close Brackets Test") {
    SP::Lexer lex;
    lex.initialize("(x == 0 { count = count + 1; call readPoint; }");
    int statement_count = 1;
    REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
    REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Branch Token Test") {
	SP::Lexer lex;
	lex.initialize("(x == 0) { }");
	int statement_count = 1;
	REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::ParseException);
	REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Missing Opening Brackets Token Test") {
	SP::Lexer lex;
	lex.initialize("(x == 0)  count = count + 1; call readPoint; }");
	int statement_count = 1;
	REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::TokenizationException);
	REQUIRE_EQUALS(statement_count, 2);
}

TEST_CASE("SP::Node::WhileNode::parseWhileStatement Closing Opening Brackets Token Test") {
	SP::Lexer lex;
	lex.initialize("(x == 0) { count = count + 1; call readPoint; ");
	int statement_count = 1;
	REQUIRE_THROWS_AS(WhileNode::parseWhileStatement(lex, statement_count), SP::ParseException);
	REQUIRE_EQUALS(statement_count, 4);
}

TEST_CASE("WhileNode::extract Test") {
	PKB pkb;
	unique_ptr<ExpressionNode> condition = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "<", "0" ,")"})));
	unique_ptr<StatementListNode> body = createStatementList("read x; print y; }", 2);
	WhileNode node = WhileNode(1, std::move(condition), std::move(body));
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, 1);
}
