#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Types;
using namespace Executor::ProcedureVariableExecutor;

TEST_CASE("ProcedureVariableExecutor<ClauseType::UsesP>:execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setUses(1, "x");
	pkb.setUses(1, "z");
	pkb.setUses(2, "x");
	pkb.setProc("A", 1, 2);
	pkb.setProc("B", 3, 4);
	pkb.populateComplexRelations();

	ReferenceArgument left_proc_no1 = ReferenceArgument("A");
	ReferenceArgument left_proc_no2 = ReferenceArgument("B");
	ReferenceArgument left_proc_no3 = ReferenceArgument(Declaration{DesignEntity::Procedure, "procedure"});
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument var = ReferenceArgument(Declaration{DesignEntity::Variable, "var"});

	SECTION("Trivial: Name & Name") {
		QueryResult result1 = executeTrivialNameName<ClauseType::UsesP>(store, left_proc_no1, x);
		QueryResult result2 = executeTrivialNameName<ClauseType::UsesP>(store, left_proc_no1, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Name & Wildcard") {
		QueryResult result1 = executeTrivialNameWildcardOrSynonym<ClauseType::UsesP>(store, left_proc_no1);
		QueryResult result2 = executeTrivialNameWildcardOrSynonym<ClauseType::UsesP>(store, left_proc_no2);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Name & Synonym") {
		QueryResult result1 = executeTrivialNameWildcardOrSynonym<ClauseType::UsesP>(store, left_proc_no1);
		QueryResult result2 = executeTrivialNameWildcardOrSynonym<ClauseType::UsesP>(store, left_proc_no2);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Name") {
		QueryResult result1 = executeTrivialSynonymName<ClauseType::UsesP>(store, x);
		QueryResult result2 = executeTrivialSynonymName<ClauseType::UsesP>(store, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesP>(store);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QueryResult result1 = executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesP>(store);

		REQUIRE(result1.getResult());
	}

	SECTION("Name & Synonym") {
		QueryResult result1 = executeNameSynonym<ClauseType::UsesP>(store, left_proc_no1, var);
		QueryResult result2 = executeNameSynonym<ClauseType::UsesP>(store, left_proc_no2, var);

		vector<string> expected_result1 = {"x", "z"};
		vector<string> actual_result1 = result1.getSynonymResult("var");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Name") {
		QueryResult result1 = executeSynonymName<ClauseType::UsesP>(store, left_proc_no3, x);
		QueryResult result2 = executeSynonymName<ClauseType::UsesP>(store, left_proc_no3, y);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard<ClauseType::UsesP>(store, left_proc_no3);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
	}

	SECTION("Synonym & Synonym") {
		QueryResult result1 = executeSynonymSynonym<ClauseType::UsesP>(store, left_proc_no3, var);

		vector<string> expected_proc_result1 = {"A", "A"};
		vector<string> actual_proc_result1 = result1.getSynonymResult("procedure");
		sort(actual_proc_result1.begin(), actual_proc_result1.end());
		vector<string> expected_var_result1 = {"x", "z"};
		vector<string> actual_var_result1 = result1.getSynonymResult("var");
		sort(actual_var_result1.begin(), actual_var_result1.end());
		REQUIRE(actual_proc_result1 == expected_proc_result1);
		REQUIRE(actual_var_result1 == expected_var_result1);
	}
}