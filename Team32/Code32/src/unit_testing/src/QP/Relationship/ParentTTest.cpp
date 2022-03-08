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
	
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("trivial: stmtNumber & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmt_no1, stmt_no3);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmt_no2, stmt_no1);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & wildcard") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmt_no1, wildcard);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmt_no3, wildcard);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmt_no1, if_synonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmt_no1, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: wildcard & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(wildcard, stmt_no3);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(wildcard, stmt_no1);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: wildcard & wildcard") {
		QP::Relationship::ParentT parent = QP::Relationship::ParentT(wildcard, wildcard);

		QP::QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: wildcard & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(wildcard, stmt_synonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(wildcard, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assign_synonym, stmt_no4);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(if_synonym, stmt_no3);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & wildcard") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assign_synonym, wildcard);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(if_synonym, wildcard);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assign_synonym, if_synonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(if_synonym, stmt_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, true, map);
		QP::QueryResult result2 = parent2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(assign_synonym, stmt_no4);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(if_synonym, stmt_no2);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & wildcard") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmt_synonym, wildcard);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(if_synonym, wildcard);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmt_synonym, if_synonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(if_synonym, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_stmt_result = {"1", "2"};
		vector<string> expected_if_result = {"4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: wildcard & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(wildcard, if_synonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(wildcard, assign_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::ParentT parent1 = QP::Relationship::ParentT(stmt_no1, stmt_synonym);
		QP::Relationship::ParentT parent2 = QP::Relationship::ParentT(stmt_no3, if_synonym);

		QP::QueryResult result1 = parent1.execute(pkb, false, map);
		QP::QueryResult result2 = parent2.execute(pkb, false, map);

		vector<string> expected_result = {"2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
