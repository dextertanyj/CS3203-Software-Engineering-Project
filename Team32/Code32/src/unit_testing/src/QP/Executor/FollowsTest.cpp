#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Executor::StatementExecutor;
using namespace Types;

TEST_CASE("StatementExecutor<ClauseType::Follows>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::While);
	pkb.setStmtType(4, StmtType::If);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);

	ClauseArgument stmt_no1 = ClauseArgument(1);
	ClauseArgument stmt_no2 = ClauseArgument(2);
	ClauseArgument stmt_no3 = ClauseArgument(3);
	ClauseArgument stmt_no4 = ClauseArgument(4);
	ClauseArgument stmt_synonym = ClauseArgument(Declaration{DesignEntity::Stmt, "s"});
	ClauseArgument assign_synonym = ClauseArgument(Declaration{DesignEntity::Assign, "a"});
	ClauseArgument if_synonym = ClauseArgument(Declaration{DesignEntity::If, "if"});
	ClauseArgument wildcard = ClauseArgument();

	SECTION("Trivial: Index & Index") {
		QueryResult result1 = executeTrivialIndexIndex<ClauseType::Follows>(store, stmt_no1, stmt_no2);
		QueryResult result2 = executeTrivialIndexIndex<ClauseType::Follows>(store, stmt_no1, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QueryResult result1 = executeTrivialIndexWildcard<ClauseType::Follows>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcard<ClauseType::Follows>(store, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QueryResult result1 = executeTrivialIndexSynonym<ClauseType::Follows>(store, stmt_no1, stmt_synonym);
		QueryResult result2 = executeTrivialIndexSynonym<ClauseType::Follows>(store, stmt_no1, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QueryResult result1 = executeTrivialWildcardIndex<ClauseType::Follows>(store, stmt_no2);
		QueryResult result2 = executeTrivialWildcardIndex<ClauseType::Follows>(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QueryResult result = executeTrivialWildcardWildcard<ClauseType::Follows>(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::Follows>(store, stmt_synonym);
		QueryResult result2 = executeTrivialWildcardSynonym<ClauseType::Follows>(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QueryResult result1 = executeTrivialSynonymIndex<ClauseType::Follows>(store, assign_synonym, stmt_no2);
		QueryResult result2 = executeTrivialSynonymIndex<ClauseType::Follows>(store, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::Follows>(store, assign_synonym);
		QueryResult result2 = executeTrivialSynonymWildcard<ClauseType::Follows>(store, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QueryResult result1 = executeTrivialSynonymSynonym<ClauseType::Follows>(store, assign_synonym, stmt_synonym);
		QueryResult result2 = executeTrivialSynonymSynonym<ClauseType::Follows>(store, assign_synonym, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QueryResult result1 = executeSynonymIndex<ClauseType::Follows>(store, assign_synonym, stmt_no2);
		QueryResult result2 = executeSynonymIndex<ClauseType::Follows>(store, if_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard<ClauseType::Follows>(store, stmt_synonym);
		QueryResult result2 = executeSynonymWildcard<ClauseType::Follows>(store, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QueryResult result1 = executeSynonymSynonym<ClauseType::Follows>(store, stmt_synonym, if_synonym);
		QueryResult result2 = executeSynonymSynonym<ClauseType::Follows>(store, if_synonym, assign_synonym);

		vector<string> expected_stmt_result = {"3"};
		vector<string> expected_if_result = {"4"};
		REQUIRE(result1.getSynonymResult("s") == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QueryResult result1 = executeWildcardSynonym<ClauseType::Follows>(store, if_synonym);
		QueryResult result2 = executeWildcardSynonym<ClauseType::Follows>(store, assign_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QueryResult result1 = executeIndexSynonym<ClauseType::Follows>(store, stmt_no1, stmt_synonym);
		QueryResult result2 = executeIndexSynonym<ClauseType::Follows>(store, stmt_no1, if_synonym);

		vector<string> expected_result = {"2"};
		REQUIRE(result1.getSynonymResult("s") == expected_result);
		REQUIRE(!result2.getResult());
	}
};
