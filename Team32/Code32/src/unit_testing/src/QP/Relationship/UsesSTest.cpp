#include "QP/Relationship/UsesS.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::UsesS::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setUses(1, "x");
	pkb.setUses(1, "z");
	pkb.setUses(2, "x");
	pkb.setUses(3, "y");

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument var = ReferenceArgument({QP::Types::DesignEntity::Variable, "var"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("trivial: index & varName") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeTrivialIndexName(pkb, stmt_no1, x);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeTrivialIndexName(pkb, stmt_no1, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: index & wildcard") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeTrivialIndex(pkb, stmt_no1);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeTrivialIndex(pkb, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: index & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeTrivialIndex(pkb, stmt_no1);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeTrivialIndex(pkb, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & name") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeTrivialSynonymName(pkb, assign_synonym, x);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeTrivialSynonymName(pkb, assign_synonym, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & wildcard") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeTrivialSynonym(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeTrivialSynonym(pkb, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeTrivialSynonym(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeTrivialSynonym(pkb, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & name") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeSynonymName(pkb, assign_synonym, x);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeSynonymName(pkb, if_synonym, y);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & wildcard") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeSynonymWildcard(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeSynonymWildcard(pkb, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeSynonymSynonym(pkb, stmt_synonym, var);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeSynonymSynonym(pkb, if_synonym, var);

		vector<string> expected_stmt_result = {"1", "1", "2", "3"};
		vector<string> expected_var_result = {"x", "x", "y", "z"};
		vector<string> actual_var_result = result1.getSynonymResult("var");
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_var_result.begin(), actual_var_result.end());
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_var_result == expected_var_result);
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: index & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesS::executeIndexSynonym(pkb, stmt_no1, var);
		QP::QueryResult result2 = QP::Relationship::UsesS::executeIndexSynonym(pkb, stmt_no4, var);

		vector<string> expected_result = {"x", "z"};
		vector<string> actual_result = result1.getSynonymResult("var");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
