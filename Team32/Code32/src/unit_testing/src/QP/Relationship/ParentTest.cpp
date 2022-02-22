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
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmtNo1, stmtNo2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmtNo1, stmtNo3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmtNo1, underscore);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmtNo4, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmtNo1, stmtSynonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmtNo1, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(underscore, stmtNo2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(underscore, stmtNo1);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::Parent parent = QP::Relationship::Parent(underscore, underscore);

		QP::QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(underscore, stmtSynonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(underscore, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assignSynonym, stmtNo2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(assignSynonym, stmtNo3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assignSynonym, underscore);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(ifSynonym, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assignSynonym, stmtSynonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(ifSynonym, stmtSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assignSynonym, stmtNo2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(ifSynonym, stmtNo2);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmtSynonym, underscore);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(ifSynonym, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmtSynonym, ifSynonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(ifSynonym, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "3" };
		vector<string> expectedIfResult = { "4" };
		REQUIRE(result1.getSynonymResult("s") == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(underscore, ifSynonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(underscore, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "4" };
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmtNo1, stmtSynonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmtNo1, ifSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = { "2" };
		REQUIRE(result1.getSynonymResult("s") == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
