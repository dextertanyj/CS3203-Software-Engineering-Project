#include "QP/Relationship/FollowsT.h"
#include "PKB/PKB.h"
#include "Common/TypeDefs.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::FollowsT::execute") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);
	pkb.populateComplexRelations();

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
		FollowsT follows1 = FollowsT(stmtNo1, stmtNo3);
		FollowsT follows2 = FollowsT(stmtNo2, stmtNo1);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		FollowsT follows1 = FollowsT(stmtNo1, underscore);
		FollowsT follows2 = FollowsT(stmtNo4, underscore);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		FollowsT follows1 = FollowsT(stmtNo1, ifSynonym);
		FollowsT follows2 = FollowsT(stmtNo1, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		FollowsT follows1 = FollowsT(underscore, stmtNo3);
		FollowsT follows2 = FollowsT(underscore, stmtNo1);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		FollowsT parent = FollowsT(underscore, underscore);

		QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		FollowsT follows1 = FollowsT(underscore, stmtSynonym);
		FollowsT follows2 = FollowsT(underscore, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		FollowsT follows1 = FollowsT(assignSynonym, stmtNo4);
		FollowsT follows2 = FollowsT(ifSynonym, stmtNo3);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		FollowsT follows1 = FollowsT(assignSynonym, underscore);
		FollowsT follows2 = FollowsT(ifSynonym, underscore);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		FollowsT follows1 = FollowsT(assignSynonym, ifSynonym);
		FollowsT follows2 = FollowsT(ifSynonym, ifSynonym);

		QueryResult result1 = follows1.execute(pkb, true, map);
		QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		FollowsT follows1 = FollowsT(assignSynonym, stmtNo4);
		FollowsT follows2 = FollowsT(ifSynonym, stmtNo2);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		FollowsT follows1 = FollowsT(stmtSynonym, underscore);
		FollowsT follows2 = FollowsT(ifSynonym, underscore);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3"};
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		FollowsT follows1 = FollowsT(stmtSynonym, ifSynonym);
		FollowsT follows2 = FollowsT(ifSynonym, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "1", "2", "3"};
		vector<string> expectedIfResult = { "4", "4", "4" };
		vector<string> actualStmtResult = result1.getSynonymResult("s");
		sort(actualStmtResult.begin(), actualStmtResult.end());
		REQUIRE(actualStmtResult == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		FollowsT follows1 = FollowsT(underscore, ifSynonym);
		FollowsT follows2 = FollowsT(underscore, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "4" };
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		FollowsT follows1 = FollowsT(stmtNo1, stmtSynonym);
		FollowsT follows2 = FollowsT(stmtNo3, assignSynonym);

		QueryResult result1 = follows1.execute(pkb, false, map);
		QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "2", "3", "4" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
