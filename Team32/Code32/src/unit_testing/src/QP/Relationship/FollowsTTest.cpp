#include "QP/Relationship/FollowsT.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::FollowsT::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);
	pkb.populateComplexRelations();

	unordered_map<string, DesignEntity> map;
	map.insert({"s", DesignEntity::Stmt});
	map.insert({"a", DesignEntity::Assign});
	map.insert({"if", DesignEntity::If});

	QueryStmtRef stmt_no1 = {StmtRefType::StmtNumber, "1"};
	QueryStmtRef stmt_no2 = {StmtRefType::StmtNumber, "2"};
	QueryStmtRef stmt_no3 = {StmtRefType::StmtNumber, "3"};
	QueryStmtRef stmt_no4 = {StmtRefType::StmtNumber, "4"};
	QueryStmtRef stmt_synonym = {StmtRefType::Synonym, "s"};
	QueryStmtRef assign_synonym = {StmtRefType::Synonym, "a"};
	QueryStmtRef if_synonym = {StmtRefType::Synonym, "if"};
	QueryStmtRef underscore = {StmtRefType::Underscore, "_"};

	SECTION("trivial: stmtNumber & stmtNumber") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(stmt_no1, stmt_no3);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(stmt_no2, stmt_no1);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(stmt_no1, underscore);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(stmt_no4, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(stmt_no1, if_synonym);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(stmt_no1, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(underscore, stmt_no3);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(underscore, stmt_no1);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::FollowsT parent = QP::Relationship::FollowsT(underscore, underscore);

		QP::QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(underscore, stmt_synonym);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(underscore, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(assign_synonym, stmt_no4);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(if_synonym, stmt_no3);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(assign_synonym, underscore);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(if_synonym, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(assign_synonym, if_synonym);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(if_synonym, if_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(assign_synonym, stmt_no4);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(if_synonym, stmt_no2);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(stmt_synonym, underscore);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(if_synonym, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(stmt_synonym, if_synonym);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(if_synonym, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_stmt_result = {"1", "2", "3"};
		vector<string> expected_if_result = {"4", "4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(underscore, if_synonym);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(underscore, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::FollowsT follows1 = QP::Relationship::FollowsT(stmt_no1, stmt_synonym);
		QP::Relationship::FollowsT follows2 = QP::Relationship::FollowsT(stmt_no3, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
