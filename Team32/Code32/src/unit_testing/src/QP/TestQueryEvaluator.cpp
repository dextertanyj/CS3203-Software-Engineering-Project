#include "QP/QueryEvaluator.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/UsesS.h"
#include "Common/ExpressionProcessor/Expression.h"

#include "catch.hpp"

TEST_CASE("QP::QueryEvaluator::splitClauses Should split clauses into groups") {
	DeclarationList declarations = {
		{ DesignEntity::stmt, "s1" },
		{ DesignEntity::stmt, "s2" },
		{ DesignEntity::variable, "v" },
		{ DesignEntity::assign, "a" },
		{ DesignEntity::if_, "i" },
	};
	Declaration select = { DesignEntity::stmt, "s1" };
	QueryStmtRef s1 = { StmtRefType::synonym, "s1" };
	QueryStmtRef s2 = { StmtRefType::synonym, "s2" };
	QueryStmtRef a = { StmtRefType::synonym, "a" };
	QueryStmtRef i = { StmtRefType::synonym, "i" };
	QueryStmtRef stmtNo1 = { StmtRefType::stmtNumber, "1" };
	QueryStmtRef stmtNo2 = { StmtRefType::stmtNumber, "2" };
	QueryEntRef v = { EntRefType::synonym, "v" };

	SuchThatClauseList suchThatClauses = {
		{ make_unique<Parent>(s1, s2) },
		{ make_unique<Parent>(a, i) },
		{ make_unique<Parent>(stmtNo1, stmtNo2) },
		{ make_unique<UsesS>(a, v) },
	};
	QueryProperties properties = QueryProperties(declarations, select, suchThatClauses, {});
	vector<unordered_set<string>> synonymsInGroup = {
		{"s1", "s2"},
		{"a", "v", "i"},
	};

	PKB pkb = PKB();
	vector<pair<SuchThatClauseList, PatternClauseList>> clausesInGroup =
		QueryEvaluator(pkb).splitClauses(properties, synonymsInGroup);

	REQUIRE(clausesInGroup.size() == 3);
	REQUIRE(clausesInGroup[0].first.size() == 1);
	REQUIRE(clausesInGroup[1].first.size() == 2);
	REQUIRE(clausesInGroup[2].first.size() == 1);
};

TEST_CASE("QP::QueryEvaluator::execute") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setUses(1, "x");
	
	QueryEvaluator evaluator = QueryEvaluator(pkb);
	
	DeclarationList declarations = {
		{ DesignEntity::stmt, "s1" },
		{ DesignEntity::stmt, "s2" },
		{ DesignEntity::variable, "v" },
		{ DesignEntity::assign, "a" },
	};
	Declaration selectAssign = { DesignEntity::assign, "a" };
	Declaration selectVar = { DesignEntity::variable, "v" };
	QueryEntRef v = { EntRefType::synonym, "v" };
	QueryStmtRef a = { StmtRefType::synonym, "a" };
	QueryStmtRef stmtNo1 = { StmtRefType::stmtNumber, "1" };

	SECTION("No clause, select assign statement") {
		QueryProperties properties = QueryProperties(declarations, selectAssign, {}, {});
		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1", "3" };
		vector<string> actualResult = result.getSynonymResult("a");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("No clause, select variable") {
		QueryProperties properties = QueryProperties(declarations, selectVar, {}, {});
		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "x" };
		vector<string> actualResult = result.getSynonymResult("v");
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("One non-trivial such that clause") {
		SuchThatClauseList suchThatClauses = {{ make_unique<UsesS>(a, v) }};
		QueryProperties properties = QueryProperties(declarations, selectAssign, suchThatClauses, {});

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1" };
		vector<string> actualResult = result.getSynonymResult("a");
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("One trivial such that clause") {
		SuchThatClauseList suchThatClauses = {{ make_unique<UsesS>(stmtNo1, v)}};
		QueryProperties properties = QueryProperties(declarations, selectAssign, suchThatClauses, {});

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1", "3" };
		vector<string> actualResult = result.getSynonymResult("a");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
	};
};
