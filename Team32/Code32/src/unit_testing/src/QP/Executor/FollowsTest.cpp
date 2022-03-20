#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor;

TEST_CASE("StatementExecutor<ClauseType::Follows>::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeTrivialIndexIndex(store, stmt_no1, stmt_no2);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeTrivialIndexIndex(store, stmt_no1, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeTrivialIndexWildcard(store, stmt_no1);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeTrivialIndexWildcard(store, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::Follows>::executeTrivialIndexSynonym(store, stmt_no1, stmt_synonym);
		QP::QueryResult result2 =
			StatementExecutor<ClauseType::Follows>::executeTrivialIndexSynonym(store, stmt_no1, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeTrivialWildcardIndex(store, stmt_no2);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeTrivialWildcardIndex(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result = StatementExecutor<ClauseType::Follows>::executeTrivialWildcardWildcard(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeTrivialWildcardSynonym(store, stmt_synonym);
		QP::QueryResult result2 =
			StatementExecutor<ClauseType::Follows>::executeTrivialWildcardSynonym(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::Follows>::executeTrivialSynonymIndex(store, assign_synonym, stmt_no2);
		QP::QueryResult result2 =
			StatementExecutor<ClauseType::Follows>::executeTrivialSynonymIndex(store, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::Follows>::executeTrivialSynonymWildcard(store, assign_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeTrivialSynonymWildcard(store, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::Follows>::executeTrivialSynonymSynonym(store, assign_synonym, stmt_synonym);
		QP::QueryResult result2 =
			StatementExecutor<ClauseType::Follows>::executeTrivialSynonymSynonym(store, assign_synonym, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::Follows>::executeSynonymIndex(store, assign_synonym, stmt_no2);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeSynonymIndex(store, if_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeSynonymWildcard(store, stmt_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeSynonymWildcard(store, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::Follows>::executeSynonymSynonym(store, stmt_synonym, if_synonym);
		QP::QueryResult result2 =
			StatementExecutor<ClauseType::Follows>::executeSynonymSynonym(store, if_synonym, assign_synonym);

		vector<string> expected_stmt_result = {"3"};
		vector<string> expected_if_result = {"4"};
		REQUIRE(result1.getSynonymResult("s") == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeWildcardSynonym(store, if_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeWildcardSynonym(store, assign_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::Follows>::executeIndexSynonym(store, stmt_no1, stmt_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::Follows>::executeIndexSynonym(store, stmt_no1, if_synonym);

		vector<string> expected_result = {"2"};
		REQUIRE(result1.getSynonymResult("s") == expected_result);
		REQUIRE(!result2.getResult());
	}
};