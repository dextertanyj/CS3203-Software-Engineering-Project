#include "QP/Relationship/ParentT.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::ParentT::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setParent(1, 2);
	pkb.setParent(1, 3);
	pkb.setParent(2, 4);
	pkb.populateComplexRelations();

	unordered_map<string, DesignEntity> map;
	map.insert({"s", DesignEntity::Stmt});
	map.insert({"a", DesignEntity::Assign});
	map.insert({"if", DesignEntity::If});

	QueryStmtRef stmtNo1 = {StmtRefType::StmtNumber, "1"};
	QueryStmtRef stmtNo2 = {StmtRefType::StmtNumber, "2"};
	QueryStmtRef stmtNo3 = {StmtRefType::StmtNumber, "3"};
	QueryStmtRef stmtNo4 = {StmtRefType::StmtNumber, "4"};
	QueryStmtRef stmtSynonym = {StmtRefType::Synonym, "s"};
	QueryStmtRef assignSynonym = {StmtRefType::Synonym, "a"};
	QueryStmtRef ifSynonym = {StmtRefType::Synonym, "if"};
	QueryStmtRef underscore = {StmtRefType::Underscore, "_"};

	SECTION("trivial: stmtNumber & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmtNo1, stmtNo3);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmtNo2, stmtNo1);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmtNo1, underscore);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmtNo3, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmtNo1, ifSynonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmtNo1, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(underscore, stmtNo3);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(underscore, stmtNo1);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::ParentT parent = QP::Relationship::ParentT(underscore, underscore);

		QP::QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(underscore, stmtSynonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(underscore, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assignSynonym, stmtNo4);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(ifSynonym, stmtNo3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assignSynonym, underscore);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(ifSynonym, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assignSynonym, ifSynonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(ifSynonym, stmtSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assignSynonym, stmtNo4);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(ifSynonym, stmtNo2);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmtSynonym, underscore);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(ifSynonym, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = {"1", "2"};
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmtSynonym, ifSynonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(ifSynonym, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedStmtResult = {"1", "2"};
		vector<string> expectedIfResult = {"4", "4"};
		vector<string> actualStmtResult = result1.getSynonymResult("s");
		sort(actualStmtResult.begin(), actualStmtResult.end());
		REQUIRE(actualStmtResult == expectedStmtResult);
		REQUIRE(result1.getSynonymResult("if") == expectedIfResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(underscore, ifSynonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(underscore, assignSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmtNo1, stmtSynonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmtNo3, ifSynonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expectedResult = {"2", "3", "4"};
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
