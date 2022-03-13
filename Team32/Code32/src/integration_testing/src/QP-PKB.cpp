#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "PKB/Storage.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryProcessor.h"
#include "catch.hpp"

TEST_CASE("Basic select") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setConstant({1, 4, 90, 2000});
	pkb.setUses(3, "x");

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Select statement") {
		string query = "stmt s; Select s";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2", "3", "4"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Select if statement") {
		string query = "if if; assign a; Select if";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"4"};
		REQUIRE(result == expected_result);
	}

	SECTION("Select variable") {
		string query = "variable v; Select v";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"x"};
		REQUIRE(result == expected_result);
	}

	SECTION("Select constant") {
		string query = "constant c; Select c";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2000", "4", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}
};

TEST_CASE("One such that clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setConstant({1, 4, 90, 2000});
	pkb.setUses(1, "x");
	pkb.setParent(2, 3);
	pkb.setFollows(1, 2);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Trivial clause") {
		string query = "constant c; Select c such that Parent(2, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2000", "4", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Trivial clause, no result") {
		string query = "constant c; assign a; Select c such that Uses(a, \"y\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}

	SECTION("Non trivial clause") {
		string query = "stmt s; variable v; Select v such that Uses(s, v)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"x"};
		REQUIRE(result == expected_result);
	}

	SECTION("Non trivial clause, no result") {
		string query = "stmt s; assign a; Select s such that Parent*(s, a)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}
};

TEST_CASE("One pattern clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setConstant({1, 90});

	vector<string> assign_token1 = {"90"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = Common::ExpressionProcessor::Expression::parse(lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assign_token2);
	auto expression2 = Common::ExpressionProcessor::Expression::parse(lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(2, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assign_token3);
	auto expression3 = Common::ExpressionProcessor::Expression::parse(lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(3, "x", expression3);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Trivial clause") {
		string query = "constant c; assign a; variable v; Select c pattern a (v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	};

	SECTION("Trivial clause, no result") {
		string query = "constant c; assign a; variable v; Select c pattern a (v, \"x\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}

	SECTION("Non-trivial clause") {
		string query = "assign a; variable v; Select a pattern a (v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"2", "3"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Non-trivial clause, no result") {
		string query = "assign a; variable v; Select a pattern a (v, \"x\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}
};

TEST_CASE("One such that and one pattern") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setStmtType(5, StmtType::Assign);

	pkb.setConstant({1, 90});
	pkb.setParent(2, 3);
	pkb.setFollows(1, 2);
	pkb.setFollows(3, 4);
	pkb.setFollows(4, 5);

	pkb.setUses(4, "x");
	pkb.setUses(5, "x");
	pkb.setUses(5, "y");

	vector<string> assign_token1 = {"90"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = Common::ExpressionProcessor::Expression::parse(lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assign_token2);
	auto expression2 = Common::ExpressionProcessor::Expression::parse(lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(4, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assign_token3);
	auto expression3 = Common::ExpressionProcessor::Expression::parse(lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(5, "x", expression3);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Both trivial") {
		string query = "constant c; stmt s1, s2; assign a; Select c such that Parent(s1, s2) pattern a(_, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	};

	SECTION("Forms a trivial group") {
		string query = "constant c; stmt s1, s2; assign a; Select c such that Follows(s1, a) pattern a(_, _\"0\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}

	SECTION("Non-trivial such that, trivial pattern") {
		string query = "stmt s1, s2; assign a; Select s1 such that Follows(s1, s2) pattern a(_, _\"1\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "3", "4"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Trivial such that, non-trivial pattern") {
		string query = "stmt s1, s2; assign a; variable v; Select a such that Follows(s1, s2) pattern a(v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"4", "5"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Both non-trivial") {
		string query = "assign a; variable v; Select a such that Uses(a, v) pattern a(v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"5"};
		REQUIRE(result == expected_result);
	}
}
