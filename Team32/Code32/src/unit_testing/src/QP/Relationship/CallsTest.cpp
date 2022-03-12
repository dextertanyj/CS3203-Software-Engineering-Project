#include "QP/Relationship/Calls.h"
#include "QP/Dispatcher/CallsDispatcher.tpp"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

using namespace QP::Types;

TEST_CASE("QP::Relationship::CallsHandler<QP::Types::ClauseType::Call>::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter pkbA = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Call);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setProc("proc1", 1, 2);
	pkb.setProc("proc2", 3, 3);
	pkb.setProc("proc3", 4, 4);
	pkb.setCall(2, "proc2");
	pkb.setCall(3, "proc3");
	pkb.populateComplexRelations();

	ReferenceArgument proc1 = ReferenceArgument("proc1");
	ReferenceArgument proc2 = ReferenceArgument("proc2");
	ReferenceArgument proc3 = ReferenceArgument("proc3");
	ReferenceArgument proc_synonym_1 = ReferenceArgument({DesignEntity::Procedure, "p1"});
	ReferenceArgument proc_synonym_2 = ReferenceArgument({DesignEntity::Procedure, "p2"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Name & Name") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialNameName(pkbA, proc1, proc2);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialNameName(pkbA, proc1, proc3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Name & Wildcard Or Synonym") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialNameWildcardOrSynonym(pkbA, proc1);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialNameWildcardOrSynonym(pkbA, proc3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard Or Synonym & Name") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialWildcardOrSynonymName(pkbA, proc2);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialWildcardOrSynonymName(pkbA, proc1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard Or Synonym & Wildcard Or Synonym But Not Both Synonym") {
		QP::QueryResult result = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkbA);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialSynonymSynonym(pkbA, proc_synonym_1, proc_synonym_2);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeTrivialSynonymSynonym(pkbA, proc_synonym_1, proc_synonym_1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Name & Synonym") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeNameSynonym(pkbA, proc1, proc_synonym_1);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeNameSynonym(pkbA, proc3, proc_synonym_1);

		vector<string> expected_result = {"proc2"};
		REQUIRE(result1.getSynonymResult("p1") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeWildcardSynonym(pkbA, proc_synonym_1);

		vector<string> expected_result = {"proc2", "proc3"};
		vector<string> actual_result = result.getSynonymResult("p1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("Synonym & Name") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeSynonymName(pkbA, proc_synonym_1, proc2);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeSynonymName(pkbA, proc_synonym_1, proc1);

		vector<string> expected_result = {"proc1"};
		REQUIRE(result1.getSynonymResult("p1") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeSynonymWildcard(pkbA, proc_synonym_1);

		vector<string> expected_result = {"proc1", "proc2"};
		vector<string> actual_result = result.getSynonymResult("p1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeSynonymSynonym(pkbA, proc_synonym_1, proc_synonym_2);
		QP::QueryResult result2 = QP::Relationship::CallsExecutor<QP::Types::ClauseType::Call>::executeSynonymSynonym(pkbA, proc_synonym_1, proc_synonym_1);

		vector<string> expected_result_1 = {"proc1", "proc2"};
		vector<string> expected_result_2 = {"proc2", "proc3"};
		vector<string> actual_result_1 = result1.getSynonymResult("p1");
		vector<string> actual_result_2 = result1.getSynonymResult("p2");
		sort(actual_result_1.begin(), actual_result_1.end());
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_1 == expected_result_1);
		REQUIRE(actual_result_2 == expected_result_2);
		REQUIRE(!result2.getResult());
	}
}
