#include <string>
#include <vector>

#include "QP/QueryProcessor.h"
#include "PKB/PKB.h"

#include "catch.hpp"

TEST_CASE("Basic select") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setConstant({ 1, 4, 90, 2000 });
	pkb.setUses(3, "x");

	QueryProcessor processor = QueryProcessor(pkb);

	SECTION("Select statement") {
		string query = "stmt s; Select s";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = { "1", "2", "3", "4" };
		sort(result.begin(), result.end());
		REQUIRE(result == expectedResult);
	}

	SECTION("Select if statement") {
		string query = "if if; assign a; Select if";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = { "4" };
		REQUIRE(result == expectedResult);
	}

	SECTION("Select variable") {
		string query = "variable v; Select v";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = { "x" };
		REQUIRE(result == expectedResult);
	}

	SECTION("Select constant") {
		string query = "constant c; Select c";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = { "1", "2000", "4", "90" };
		sort(result.begin(), result.end());
		REQUIRE(result == expectedResult);
	}
};

TEST_CASE("One such that clause") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setConstant({ 1, 4, 90, 2000 });
	pkb.setUses(1, "x");
	pkb.setParent(2, 3);
	pkb.setFollows(1, 2);

	QueryProcessor processor = QueryProcessor(pkb);

	SECTION("Trivial clause") {
		string query = "constant c; Select c such that Parent(2, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = { "1", "2000", "4", "90" };
		sort(result.begin(), result.end());
		REQUIRE(result == expectedResult);
	}

	SECTION("Trivial clause, no result") {
		string query = "constant c; assign a; Select c such that Uses(a, \"y\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = {};
		REQUIRE(result == expectedResult);
	}

	SECTION("Non trivial clause") {
		string query = "stmt s; variable v; Select v such that Uses(s, v)";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = { "x" };
		REQUIRE(result == expectedResult);
	}

	SECTION("Non trivial clause, no result") {
		string query = "stmt s; assign a; Select s such that Parent*(s, a)";

		vector<string> result = processor.processQuery(query);

		vector<string> expectedResult = {};
		REQUIRE(result == expectedResult);
	}
};
