#include "QP/Relationship/ParentT.h"
#include "PKB/PKB.h"
#include "Common/TypeDefs.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::ParentT::execute") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setParent(1, 2);
	pkb.setParent(1, 3);
	pkb.setParent(2, 4);
	pkb.populateComplexRelations();

	unordered_map<string, DesignEntity> map;
	map.insert({ "s", DesignEntity::stmt });
	map.insert({ "a", DesignEntity::assign });
	map.insert({ "if", DesignEntity::if_ });

	QueryStmtRef stmtNo1 = { StmtRefType::stmtNumber, "1" };
	QueryStmtRef stmtNo2 = { StmtRefType::stmtNumber, "2" };
	QueryStmtRef stmtNo3 = { StmtRefType::stmtNumber, "3" };
	QueryStmtRef stmtNo4 = { StmtRefType::stmtNumber, "4" };
	QueryStmtRef stmtSynonym = { StmtRefType::synonym, "s" };
	QueryStmtRef assignSynonym = { StmtRefType::synonym, "a" };
	QueryStmtRef ifSynonym = { StmtRefType::synonym, "if" };
	QueryStmtRef underscore = { StmtRefType::underscore, "_"};

	SECTION("trivial: stmtNumber & stmtNumber") {
		ParentT parent1 = ParentT(stmtNo1, stmtNo3);
		ParentT parent2 = ParentT(stmtNo2, stmtNo1);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		ParentT parent1 = ParentT(stmtNo1, underscore);
		ParentT parent2 = ParentT(stmtNo3, underscore);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		ParentT parent1 = ParentT(stmtNo1, ifSynonym);
		ParentT parent2 = ParentT(stmtNo1, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		ParentT parent1 = ParentT(underscore, stmtNo3);
		ParentT parent2 = ParentT(underscore, stmtNo1);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		ParentT parent = ParentT(underscore, underscore);

		QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		ParentT parent1 = ParentT(underscore, stmtSynonym);
		ParentT parent2 = ParentT(underscore, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		ParentT parent1 = ParentT(assignSynonym, stmtNo4);
		ParentT parent2 = ParentT(ifSynonym, stmtNo3);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		ParentT parent1 = ParentT(assignSynonym, underscore);
		ParentT parent2 = ParentT(ifSynonym, underscore);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		ParentT parent1 = ParentT(assignSynonym, stmtNo4);
		ParentT parent2 = ParentT(ifSynonym, stmtNo2);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		ParentT parent1 = ParentT(stmtSynonym, underscore);
		ParentT parent2 = ParentT(ifSynonym, underscore);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		ParentT parent1 = ParentT(stmtSynonym, ifSynonym);
		ParentT parent2 = ParentT(ifSynonym, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "1", "2" };
		vector<string> expectedIfResult = { "4", "4"};
		vector<string> actualStmtResult = result1.getSynonymResult("s");
		sort(actualStmtResult.begin(), actualStmtResult.end());
		REQUIRE(actualStmtResult == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		ParentT parent1 = ParentT(underscore, ifSynonym);
		ParentT parent2 = ParentT(underscore, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "4" };
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		ParentT parent1 = ParentT(stmtNo1, stmtSynonym);
		ParentT parent2 = ParentT(stmtNo3, ifSynonym);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "2", "3", "4" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
