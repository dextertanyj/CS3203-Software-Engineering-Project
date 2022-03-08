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

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("trivial: stmtNumber & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_no1, stmt_no2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmt_no1, stmt_no3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & wildcard") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_no1, wildcard);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(stmt_no4, wildcard);

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

	SECTION("trivial: wildcard & stmtNumber") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(wildcard, stmt_no2);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(wildcard, stmt_no1);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: wildcard & wildcard") {
		QP::Relationship::Parent parent = QP::Relationship::Parent(wildcard, wildcard);

		QP::QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: wildcard & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(wildcard, stmt_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(wildcard, assign_synonym);

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

	SECTION("trivial: synonym & wildcard") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(assign_synonym, wildcard);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, wildcard);

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

	SECTION("non-trivial: synonym & wildcard") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(stmt_synonym, wildcard);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(if_synonym, wildcard);

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

	SECTION("non-trivial: wildcard & synonym") {
		QP::Relationship::Parent parent1 = QP::Relationship::Parent(wildcard, if_synonym);
		QP::Relationship::Parent parent2 = QP::Relationship::Parent(wildcard, assign_synonym);

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
