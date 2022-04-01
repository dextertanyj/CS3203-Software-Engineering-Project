#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor::StatementExecutor;

TEST_CASE("StatementExecutor<ClauseType::NextT>:execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::Read);
	pkb.setNext(1, 2);
	pkb.setNext(2, 3);
	pkb.setNext(3, 4);
	pkb.setNext(4, 3);
	pkb.setNext(3, 5);
	pkb.populateComplexRelations();

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_no5 = ReferenceArgument(5);
	ReferenceArgument stmt_synonym = ReferenceArgument(Declaration{DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument(Declaration{DesignEntity::Assign, "a"});
	ReferenceArgument read_synonym = ReferenceArgument(Declaration{DesignEntity::Read, "r"});
	ReferenceArgument while_synonym = ReferenceArgument(Declaration{DesignEntity::While, "w"});
	ReferenceArgument print_synonym = ReferenceArgument(Declaration{DesignEntity::Print, "p"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QP::QueryResult result1 = executeTrivialIndexIndex<ClauseType::NextT>(store, stmt_no1, stmt_no3);
		QP::QueryResult result2 = executeTrivialIndexIndex<ClauseType::NextT>(store, stmt_no2, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = executeTrivialIndexWildcard<ClauseType::NextT>(store, stmt_no1);
		QP::QueryResult result2 = executeTrivialIndexWildcard<ClauseType::NextT>(store, stmt_no5);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = executeTrivialIndexSynonym<ClauseType::NextT>(store, stmt_no1, print_synonym);
		QP::QueryResult result2 = executeTrivialIndexSynonym<ClauseType::NextT>(store, stmt_no1, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = executeTrivialWildcardIndex<ClauseType::NextT>(store, stmt_no3);
		QP::QueryResult result2 = executeTrivialWildcardIndex<ClauseType::NextT>(store, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result = executeTrivialWildcardWildcard<ClauseType::NextT>(store);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::NextT>(store, stmt_synonym);
		QP::QueryResult result2 = executeTrivialWildcardSynonym<ClauseType::NextT>(store, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 = executeTrivialSynonymIndex<ClauseType::NextT>(store, assign_synonym, stmt_no4);
		QP::QueryResult result2 = executeTrivialSynonymIndex<ClauseType::NextT>(store, print_synonym, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::NextT>(store, assign_synonym);
		QP::QueryResult result2 = executeTrivialSynonymWildcard<ClauseType::NextT>(store, read_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 = executeTrivialSynonymSynonym<ClauseType::NextT>(store, assign_synonym, print_synonym);
		QP::QueryResult result2 = executeTrivialSynonymSynonym<ClauseType::NextT>(store, while_synonym, while_synonym);
		QP::QueryResult result3 = executeTrivialSynonymSynonym<ClauseType::NextT>(store, assign_synonym, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 = executeSynonymIndex<ClauseType::NextT>(store, assign_synonym, stmt_no4);
		QP::QueryResult result2 = executeSynonymIndex<ClauseType::NextT>(store, while_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = executeSynonymWildcard<ClauseType::NextT>(store, stmt_synonym);
		QP::QueryResult result2 = executeSynonymWildcard<ClauseType::NextT>(store, read_synonym);

		vector<string> expected_result = {"1", "2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = executeSynonymSynonymOptimized<ClauseType::NextT>(store, {}, stmt_synonym, print_synonym);
		QP::QueryResult result2 = executeSynonymSynonymOptimized<ClauseType::NextT>(store, {}, while_synonym, while_synonym);
		QP::QueryResult result3 = executeSynonymSynonymOptimized<ClauseType::NextT>(store, {}, print_synonym, assign_synonym);

		vector<string> expected_stmt_result = {"1", "1", "2", "3", "4"};
		vector<string> expected_print_result = {"2", "4", "4", "4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		vector<string> actual_print_result = result1.getSynonymResult("p");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		sort(actual_print_result.begin(), actual_print_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(actual_print_result == expected_print_result);

		vector<string> expected_while_result = {"3"};
		REQUIRE(result2.getSynonymResult("w") == expected_while_result);

		REQUIRE(!result3.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result1 = executeWildcardSynonym<ClauseType::NextT>(store, print_synonym);
		QP::QueryResult result2 = executeWildcardSynonym<ClauseType::NextT>(store, assign_synonym);

		vector<string> expected_result = {"2", "4"};
		vector<string> actual_result = result1.getSynonymResult("p");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = executeIndexSynonym<ClauseType::NextT>(store, stmt_no1, stmt_synonym);
		QP::QueryResult result2 = executeIndexSynonym<ClauseType::NextT>(store, stmt_no3, assign_synonym);

		vector<string> expected_result = {"2", "3", "4", "5"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
