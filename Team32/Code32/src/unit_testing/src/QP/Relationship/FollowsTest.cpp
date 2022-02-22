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
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmtNo1, stmtNo2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmtNo1, stmtNo3);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmtNo1, underscore);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmtNo4, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmtNo1, stmtSynonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmtNo1, assignSynonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(underscore, stmtNo2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(underscore, stmtNo1);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::Follows follows = QP::Relationship::Follows(underscore, underscore);

		QP::QueryResult result = follows.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(underscore, stmtSynonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(underscore, assignSynonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assignSynonym, stmtNo2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(assignSynonym, stmtNo3);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assignSynonym, underscore);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(ifSynonym, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assignSynonym, stmtSynonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(assignSynonym, ifSynonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assignSynonym, stmtNo2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(ifSynonym, stmtNo2);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmtSynonym, underscore);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(ifSynonym, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmtSynonym, ifSynonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(ifSynonym, assignSynonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "3" };
		vector<string> expectedIfResult = { "4" };
		REQUIRE(result1.getSynonymResult("s") == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(underscore, ifSynonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(underscore, assignSynonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "4" };
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmtNo1, stmtSynonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmtNo1, ifSynonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expectedResult = { "2" };
		REQUIRE(result1.getSynonymResult("s") == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
