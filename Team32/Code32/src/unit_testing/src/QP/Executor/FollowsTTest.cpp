#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Executor::StatementExecutor;
using namespace Types;

TEST_CASE("StatementExecutor<ClauseType::FollowsT>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::If);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);
	pkb.populateComplexRelations();

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument(Declaration{DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument(Declaration{DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument(Declaration{DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QueryResult result1 = executeTrivialIndexIndex<ClauseType::FollowsT>(store, stmt_no1, stmt_no3);
		QueryResult result2 = executeTrivialIndexIndex<ClauseType::FollowsT>(store, stmt_no2, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QueryResult result1 = executeTrivialIndexWildcard<ClauseType::FollowsT>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcard<ClauseType::FollowsT>(store, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QueryResult result1 = executeTrivialIndexSynonym<ClauseType::FollowsT>(store, stmt_no1, if_synonym);
		QueryResult result2 = executeTrivialIndexSynonym<ClauseType::FollowsT>(store, stmt_no1, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QueryResult result1 = executeTrivialWildcardIndex<ClauseType::FollowsT>(store, stmt_no3);
		QueryResult result2 = executeTrivialWildcardIndex<ClauseType::FollowsT>(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QueryResult result = executeTrivialWildcardWildcard<ClauseType::FollowsT>(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::FollowsT>(store, stmt_synonym);
		QueryResult result2 = executeTrivialWildcardSynonym<ClauseType::FollowsT>(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QueryResult result1 = executeTrivialSynonymIndex<ClauseType::FollowsT>(store, assign_synonym, stmt_no4);
		QueryResult result2 = executeTrivialSynonymIndex<ClauseType::FollowsT>(store, if_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::FollowsT>(store, assign_synonym);
		QueryResult result2 = executeTrivialSynonymWildcard<ClauseType::FollowsT>(store, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QueryResult result1 = executeTrivialSynonymSynonym<ClauseType::FollowsT>(store, assign_synonym, if_synonym);
		QueryResult result2 = executeTrivialSynonymSynonym<ClauseType::FollowsT>(store, if_synonym, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QueryResult result1 = executeSynonymIndex<ClauseType::FollowsT>(store, assign_synonym, stmt_no4);
		QueryResult result2 = executeSynonymIndex<ClauseType::FollowsT>(store, if_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard<ClauseType::FollowsT>(store, stmt_synonym);
		QueryResult result2 = executeSynonymWildcard<ClauseType::FollowsT>(store, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QueryResult result1 = executeSynonymSynonym<ClauseType::FollowsT>(store, stmt_synonym, if_synonym);
		QueryResult result2 = executeSynonymSynonym<ClauseType::FollowsT>(store, if_synonym, assign_synonym);

		vector<string> expected_stmt_result = {"1", "2", "3"};
		vector<string> expected_if_result = {"4", "4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QueryResult result1 = executeWildcardSynonym<ClauseType::FollowsT>(store, if_synonym);
		QueryResult result2 = executeWildcardSynonym<ClauseType::FollowsT>(store, assign_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QueryResult result1 = executeIndexSynonym<ClauseType::FollowsT>(store, stmt_no1, stmt_synonym);
		QueryResult result2 = executeIndexSynonym<ClauseType::FollowsT>(store, stmt_no3, assign_synonym);

		vector<string> expected_result = {"2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
