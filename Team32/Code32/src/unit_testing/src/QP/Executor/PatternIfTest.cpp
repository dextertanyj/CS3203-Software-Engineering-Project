#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Executor::PatternContainerStatementExecutor;
using namespace Types;

TEST_CASE("PatternContainerStatementExecutor<ClauseType::PatternIf>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::If);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::If);
	pkb.setStmtType(4, StmtType::WhileStmt);
	pkb.setIfControl(1, "x");
	pkb.setIfControl(1, "y");
	pkb.setIfControl(2, "z");
	pkb.setWhileControl(4, "x");
	pkb.setWhileControl(4, "w");
	pkb.setUses(1, "x");
	pkb.setUses(1, "y");
	pkb.setUses(2, "z");
	pkb.setUses(4, "x");
	pkb.setUses(4, "w");
	pkb.populateComplexRelations();

	ReferenceArgument syn_if = ReferenceArgument(Declaration{Types::DesignEntity::If, "i"});
	ReferenceArgument syn_var = ReferenceArgument(Declaration{Types::DesignEntity::Variable, "v"});
	ReferenceArgument a = ReferenceArgument("a");
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument z = ReferenceArgument("z");

	SECTION("Trivial: Name") {
		QueryResult result1 = executeTrivialName<ClauseType::PatternIf>(store, x);
		QueryResult result2 = executeTrivialName<ClauseType::PatternIf>(store, a);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard or Synonym") {
		QueryResult result1 = executeTrivialWildcardOrSynonym<ClauseType::PatternIf>(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Non-Trivial: Name") {
		QueryResult result1 = executeName<ClauseType::PatternIf>(store, syn_if, x);
		QueryResult result2 = executeName<ClauseType::PatternIf>(store, syn_if, a);

		REQUIRE(result1.getSynonymResult("i") == vector<string>{"1"});
		REQUIRE(!result2.getResult());
	}

	SECTION("Non-Trivial: Wildcard") {
		QueryResult result1 = executeWildcard<ClauseType::PatternIf>(store, syn_if);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("i");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("Non-Trivial: Synonym") {
		QueryResult result1 = executeSynonym<ClauseType::PatternIf>(store, syn_if, syn_var);

		vector<string> expected_result_i = {"1", "1", "2"};
		vector<string> actual_result_i = result1.getSynonymResult("i");
		vector<string> expected_result_v = {"x", "y", "z"};
		vector<string> actual_result_v = result1.getSynonymResult("v");
		sort(actual_result_i.begin(), actual_result_i.end());
		sort(actual_result_v.begin(), actual_result_v.end());
		REQUIRE(actual_result_i == expected_result_i);
		REQUIRE(actual_result_v == expected_result_v);
	}
}