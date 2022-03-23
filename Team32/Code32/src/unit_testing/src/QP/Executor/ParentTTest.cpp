#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor;

TEST_CASE("StatementExecutor<ClauseType::ParentT>::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::IfStmt);
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
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialIndexIndex(store, stmt_no1, stmt_no3);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialIndexIndex(store, stmt_no2, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialIndexWildcard(store, stmt_no1);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialIndexWildcard(store, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialIndexSynonym(store, stmt_no1, if_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialIndexSynonym(store, stmt_no2, while_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialWildcardIndex(store, stmt_no3);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialWildcardIndex(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result = StatementExecutor<ClauseType::ParentT>::executeTrivialWildcardWildcard(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialWildcardSynonym(store, assign_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialWildcardSynonym(store, while_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialSynonymIndex(store, while_synonym, stmt_no4);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialSynonymIndex(store, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeTrivialSynonymWildcard(store, while_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialSynonymWildcard(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 =
			StatementExecutor<ClauseType::ParentT>::executeTrivialSynonymSynonym(store, while_synonym, assign_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeTrivialSynonymSynonym(store, assign_synonym, stmt_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeSynonymIndex(store, while_synonym, stmt_no4);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeSynonymIndex(store, assign_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("w") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeSynonymWildcard(store, stmt_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeSynonymWildcard(store, assign_synonym);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeSynonymSynonym(store, stmt_synonym, assign_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeSynonymSynonym(store, assign_synonym, while_synonym);

		vector<string> expected_stmt_result = {"1", "2"};
		vector<string> expected_assign_result = {"4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("a") == expected_assign_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeWildcardSynonym(store, assign_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeWildcardSynonym(store, while_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = StatementExecutor<ClauseType::ParentT>::executeIndexSynonym(store, stmt_no1, stmt_synonym);
		QP::QueryResult result2 = StatementExecutor<ClauseType::ParentT>::executeIndexSynonym(store, stmt_no3, if_synonym);

		vector<string> expected_result = {"2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
