#include "QP/Relationship/Parent.h"
#include "PKB/Storage.h"
#include "Common/TypeDefs.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::Parent::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setParent(1, 2);
	pkb.setParent(2, 3);
	pkb.setParent(3, 4);

	unordered_map<string, DesignEntity> map;
	map.insert({ "s", DesignEntity::Stmt });
	map.insert({ "a", DesignEntity::Assign });
	map.insert({ "if", DesignEntity::If });

	QueryStmtRef stmtNo1 = { StmtRefType::StmtNumber, "1" };
	QueryStmtRef stmtNo2 = { StmtRefType::StmtNumber, "2" };
	QueryStmtRef stmtNo3 = { StmtRefType::StmtNumber, "3" };
	QueryStmtRef stmtNo4 = { StmtRefType::StmtNumber, "4" };
	QueryStmtRef stmtSynonym = { StmtRefType::Synonym, "s" };
	QueryStmtRef assignSynonym = { StmtRefType::Synonym, "a" };
	QueryStmtRef ifSynonym = { StmtRefType::Synonym, "if" };
	QueryStmtRef underscore = { StmtRefType::Underscore, "_"};

	SECTION("trivial: stmtNumber & stmtNumber") {
		Parent parent1 = Parent(stmtNo1, stmtNo2);
		Parent parent2 = Parent(stmtNo1, stmtNo3);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		Parent parent1 = Parent(stmtNo1, underscore);
		Parent parent2 = Parent(stmtNo4, underscore);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		Parent parent1 = Parent(stmtNo1, stmtSynonym);
		Parent parent2 = Parent(stmtNo1, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		Parent parent1 = Parent(underscore, stmtNo2);
		Parent parent2 = Parent(underscore, stmtNo1);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		Parent parent = Parent(underscore, underscore);

		QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		Parent parent1 = Parent(underscore, stmtSynonym);
		Parent parent2 = Parent(underscore, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		Parent parent1 = Parent(assignSynonym, stmtNo2);
		Parent parent2 = Parent(assignSynonym, stmtNo3);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		Parent parent1 = Parent(assignSynonym, underscore);
		Parent parent2 = Parent(ifSynonym, underscore);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		Parent parent1 = Parent(assignSynonym, stmtSynonym);
		Parent parent2 = Parent(ifSynonym, stmtSynonym);

		QueryResult result1 = parent1.execute(pkb, true, map);
		QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		Parent parent1 = Parent(assignSynonym, stmtNo2);
		Parent parent2 = Parent(ifSynonym, stmtNo2);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		Parent parent1 = Parent(stmtSynonym, underscore);
		Parent parent2 = Parent(ifSynonym, underscore);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		Parent parent1 = Parent(stmtSynonym, ifSynonym);
		Parent parent2 = Parent(ifSynonym, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "3" };
		vector<string> expectedIfResult = { "4" };
		REQUIRE(result1.getSynonymResult("s") == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		Parent parent1 = Parent(underscore, ifSynonym);
		Parent parent2 = Parent(underscore, assignSynonym);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "4" };
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		Parent parent1 = Parent(stmtNo1, stmtSynonym);
		Parent parent2 = Parent(stmtNo1, ifSynonym);

		QueryResult result1 = parent1.execute(pkb, false, map);
		QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "2" };
		REQUIRE(result1.getSynonymResult("s") == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
