#include "QP/Relationship/Parent.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
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
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_no1, stmt_no2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmt_no1, stmt_no3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_no1, underscore);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmt_no4, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_no1, stmt_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmt_no1, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(underscore, stmt_no2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(underscore, stmt_no1);

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
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(underscore, stmt_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(underscore, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assign_synonym, stmt_no2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(assign_synonym, stmt_no3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assign_synonym, underscore);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assign_synonym, stmt_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, stmt_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assign_synonym, stmt_no2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, stmt_no2);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_synonym, underscore);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, underscore);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_synonym, if_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_stmt_result = {"3"};
		vector<string> expected_if_result = {"4"};
		REQUIRE(result1.getSynonymResult("s") == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(underscore, if_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(underscore, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_no1, stmt_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmt_no1, if_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"2"};
		REQUIRE(result1.getSynonymResult("s") == expected_result);
		REQUIRE(!result2.getResult());
	}
};
