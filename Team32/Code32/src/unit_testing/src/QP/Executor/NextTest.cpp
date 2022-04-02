#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Types;
using namespace Executor::StatementExecutor;

TEST_CASE("StatementExecutor<ClauseType::Next>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setNext(1, 2);
	pkb.setNext(2, 3);
	pkb.setNext(3, 4);

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument(Declaration{DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument(Declaration{DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument(Declaration{DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QueryResult result1 = executeTrivialIndexIndex<ClauseType::Next>(store, stmt_no1, stmt_no2);
		QueryResult result2 = executeTrivialIndexIndex<ClauseType::Next>(store, stmt_no1, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QueryResult result1 = executeTrivialIndexWildcard<ClauseType::Next>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcard<ClauseType::Next>(store, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QueryResult result1 =
			executeTrivialIndexSynonym<ClauseType::Next>(store, stmt_no1, stmt_synonym);
		QueryResult result2 =
			executeTrivialIndexSynonym<ClauseType::Next>(store, stmt_no1, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QueryResult result1 = executeTrivialWildcardIndex<ClauseType::Next>(store, stmt_no2);
		QueryResult result2 = executeTrivialWildcardIndex<ClauseType::Next>(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QueryResult result = executeTrivialWildcardWildcard<ClauseType::Next>(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::Next>(store, stmt_synonym);
		QueryResult result2 =
			executeTrivialWildcardSynonym<ClauseType::Next>(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QueryResult result1 =
			executeTrivialSynonymIndex<ClauseType::Next>(store, assign_synonym, stmt_no2);
		QueryResult result2 =
			executeTrivialSynonymIndex<ClauseType::Next>(store, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 =
			executeTrivialSynonymWildcard<ClauseType::Next>(store, assign_synonym);
		QueryResult result2 = executeTrivialSynonymWildcard<ClauseType::Next>(store, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QueryResult result1 =
			executeTrivialSynonymSynonym<ClauseType::Next>(store, assign_synonym, stmt_synonym);
		QueryResult result2 =
			executeTrivialSynonymSynonym<ClauseType::Next>(store, assign_synonym, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QueryResult result1 =
			executeSynonymIndex<ClauseType::Next>(store, assign_synonym, stmt_no2);
		QueryResult result2 = executeSynonymIndex<ClauseType::Next>(store, if_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard<ClauseType::Next>(store, stmt_synonym);
		QueryResult result2 = executeSynonymWildcard<ClauseType::Next>(store, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QueryResult result1 =
			executeSynonymSynonym<ClauseType::Next>(store, stmt_synonym, if_synonym);
		QueryResult result2 =
			executeSynonymSynonym<ClauseType::Next>(store, if_synonym, assign_synonym);

		vector<string> expected_stmt_result = {"3"};
		vector<string> expected_if_result = {"4"};
		REQUIRE(result1.getSynonymResult("s") == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QueryResult result1 = executeWildcardSynonym<ClauseType::Next>(store, if_synonym);
		QueryResult result2 = executeWildcardSynonym<ClauseType::Next>(store, assign_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QueryResult result1 = executeIndexSynonym<ClauseType::Next>(store, stmt_no1, stmt_synonym);
		QueryResult result2 = executeIndexSynonym<ClauseType::Next>(store, stmt_no1, if_synonym);

		vector<string> expected_result = {"2"};
		REQUIRE(result1.getSynonymResult("s") == expected_result);
		REQUIRE(!result2.getResult());
	}
};
