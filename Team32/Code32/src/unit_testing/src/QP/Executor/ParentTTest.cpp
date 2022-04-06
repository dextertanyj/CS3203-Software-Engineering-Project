#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Executor::StatementExecutor;
using namespace Types;

TEST_CASE("StatementExecutor<ClauseType::ParentT>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setParent(1, 2);
	pkb.setParent(1, 3);
	pkb.setParent(2, 4);
	pkb.populateComplexRelations();

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument(Declaration{DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument(Declaration{DesignEntity::Assign, "a"});
	ReferenceArgument while_synonym = ReferenceArgument(Declaration{DesignEntity::While, "w"});
	ReferenceArgument if_synonym = ReferenceArgument(Declaration{DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QueryResult result1 = executeTrivialIndexIndex<ClauseType::ParentT>(store, stmt_no1, stmt_no3);
		QueryResult result2 = executeTrivialIndexIndex<ClauseType::ParentT>(store, stmt_no2, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QueryResult result1 = executeTrivialIndexWildcard<ClauseType::ParentT>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcard<ClauseType::ParentT>(store, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QueryResult result1 = executeTrivialIndexSynonym<ClauseType::ParentT>(store, stmt_no1, if_synonym);
		QueryResult result2 = executeTrivialIndexSynonym<ClauseType::ParentT>(store, stmt_no2, while_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QueryResult result1 = executeTrivialWildcardIndex<ClauseType::ParentT>(store, stmt_no3);
		QueryResult result2 = executeTrivialWildcardIndex<ClauseType::ParentT>(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QueryResult result = executeTrivialWildcardWildcard<ClauseType::ParentT>(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::ParentT>(store, assign_synonym);
		QueryResult result2 = executeTrivialWildcardSynonym<ClauseType::ParentT>(store, while_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QueryResult result1 = executeTrivialSynonymIndex<ClauseType::ParentT>(store, while_synonym, stmt_no4);
		QueryResult result2 = executeTrivialSynonymIndex<ClauseType::ParentT>(store, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::ParentT>(store, while_synonym);
		QueryResult result2 = executeTrivialSynonymWildcard<ClauseType::ParentT>(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QueryResult result1 =
			executeTrivialSynonymSynonym<ClauseType::ParentT>(store, while_synonym, assign_synonym);
		QueryResult result2 = executeTrivialSynonymSynonym<ClauseType::ParentT>(store, assign_synonym, stmt_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QueryResult result1 = executeSynonymIndex<ClauseType::ParentT>(store, while_synonym, stmt_no4);
		QueryResult result2 = executeSynonymIndex<ClauseType::ParentT>(store, assign_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("w") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard<ClauseType::ParentT>(store, stmt_synonym);
		QueryResult result2 = executeSynonymWildcard<ClauseType::ParentT>(store, assign_synonym);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QueryResult result1 = executeSynonymSynonym<ClauseType::ParentT>(store, stmt_synonym, assign_synonym);
		QueryResult result2 = executeSynonymSynonym<ClauseType::ParentT>(store, assign_synonym, while_synonym);

		vector<string> expected_stmt_result = {"1", "2"};
		vector<string> expected_assign_result = {"4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("a") == expected_assign_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QueryResult result1 = executeWildcardSynonym<ClauseType::ParentT>(store, assign_synonym);
		QueryResult result2 = executeWildcardSynonym<ClauseType::ParentT>(store, while_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QueryResult result1 = executeIndexSynonym<ClauseType::ParentT>(store, stmt_no1, stmt_synonym);
		QueryResult result2 = executeIndexSynonym<ClauseType::ParentT>(store, stmt_no3, if_synonym);

		vector<string> expected_result = {"2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
