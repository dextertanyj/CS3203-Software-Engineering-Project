#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Executor::StatementVariableExecutor;
using namespace Types;

TEST_CASE("StatementVariableExecutor<ClauseType::UsesS>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setUses(1, "x");
	pkb.setUses(1, "z");
	pkb.setUses(2, "x");
	pkb.setUses(3, "y");

	ClauseArgument stmt_no1 = ClauseArgument(1);
	ClauseArgument stmt_no2 = ClauseArgument(2);
	ClauseArgument stmt_no3 = ClauseArgument(3);
	ClauseArgument stmt_no4 = ClauseArgument(4);
	ClauseArgument stmt_synonym = ClauseArgument(Declaration{DesignEntity::Stmt, "s"});
	ClauseArgument assign_synonym = ClauseArgument(Declaration{DesignEntity::Assign, "a"});
	ClauseArgument if_synonym = ClauseArgument(Declaration{DesignEntity::If, "if"});
	ClauseArgument x = ClauseArgument("x");
	ClauseArgument y = ClauseArgument("y");
	ClauseArgument var = ClauseArgument(Declaration{DesignEntity::Variable, "var"});

	SECTION("Trivial: Index & Name") {
		QueryResult result1 = executeTrivialIndexName<ClauseType::UsesS>(store, stmt_no1, x);
		QueryResult result2 = executeTrivialIndexName<ClauseType::UsesS>(store, stmt_no1, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QueryResult result1 = executeTrivialIndexWildcardOrSynonym<ClauseType::UsesS>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcardOrSynonym<ClauseType::UsesS>(store, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QueryResult result1 = executeTrivialIndexWildcardOrSynonym<ClauseType::UsesS>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcardOrSynonym<ClauseType::UsesS>(store, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Name") {
		QueryResult result1 = executeTrivialSynonymName<ClauseType::UsesS>(store, assign_synonym, x);
		QueryResult result2 = executeTrivialSynonymName<ClauseType::UsesS>(store, assign_synonym, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesS>(store, stmt_synonym);
		QueryResult result2 = executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesS>(store, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QueryResult result1 = executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesS>(store, stmt_synonym);
		QueryResult result2 = executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesS>(store, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Name") {
		QueryResult result1 = executeSynonymName<ClauseType::UsesS>(store, assign_synonym, x);
		QueryResult result2 = executeSynonymName<ClauseType::UsesS>(store, if_synonym, y);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard<ClauseType::UsesS>(store, stmt_synonym);
		QueryResult result2 = executeSynonymWildcard<ClauseType::UsesS>(store, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QueryResult result1 = executeSynonymSynonym<ClauseType::UsesS>(store, stmt_synonym, var);
		QueryResult result2 = executeSynonymSynonym<ClauseType::UsesS>(store, if_synonym, var);

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

	SECTION("Index & Synonym") {
		QueryResult result1 = executeIndexSynonym<ClauseType::UsesS>(store, stmt_no1, var);
		QueryResult result2 = executeIndexSynonym<ClauseType::UsesS>(store, stmt_no4, var);

		vector<string> expected_result = {"x", "z"};
		vector<string> actual_result = result1.getSynonymResult("var");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
