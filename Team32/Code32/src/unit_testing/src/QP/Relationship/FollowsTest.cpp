#include "QP/Relationship/Follows.h"
#include "PKB/Storage.h"
#include "Common/TypeDefs.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::Follows::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);

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
		Follows follows1 = Follows(stmtNo1, stmtNo2);
		Follows follows2 = Follows(stmtNo1, stmtNo3);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		Follows follows1 = Follows(stmtNo1, underscore);
		Follows follows2 = Follows(stmtNo4, underscore);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		Follows follows1 = Follows(stmtNo1, stmtSynonym);
		Follows follows2 = Follows(stmtNo1, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		Follows follows1 = Follows(underscore, stmtNo2);
		Follows follows2 = Follows(underscore, stmtNo1);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		Follows follows = Follows(underscore, underscore);

		QueryResult result = follows.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		Follows follows1 = Follows(underscore, stmtSynonym);
		Follows follows2 = Follows(underscore, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		Follows follows1 = Follows(assignSynonym, stmtNo2);
		Follows follows2 = Follows(assignSynonym, stmtNo3);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		Follows follows1 = Follows(assignSynonym, underscore);
		Follows follows2 = Follows(ifSynonym, underscore);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		Follows follows1 = Follows(assignSynonym, stmtSynonym);
		Follows follows2 = Follows(assignSynonym, ifSynonym);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		Follows follows1 = Follows(assignSynonym, stmtNo2);
		Follows follows2 = Follows(ifSynonym, stmtNo2);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		Follows follows1 = Follows(stmtSynonym, underscore);
		Follows follows2 = Follows(ifSynonym, underscore);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		Follows follows1 = Follows(stmtSynonym, ifSynonym);
		Follows follows2 = Follows(ifSynonym, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "3" };
		vector<string> expectedIfResult = { "4" };
		REQUIRE(result1.getSynonymResult("s") == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		Follows follows1 = Follows(underscore, ifSynonym);
		Follows follows2 = Follows(underscore, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "4" };
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		Follows follows1 = Follows(stmtNo1, stmtSynonym);
		Follows follows2 = Follows(stmtNo1, ifSynonym);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "2" };
		REQUIRE(result1.getSynonymResult("s") == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
