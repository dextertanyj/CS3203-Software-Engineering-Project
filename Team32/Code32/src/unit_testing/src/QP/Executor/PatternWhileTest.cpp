#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"
#include "catch.hpp"

using namespace QP;
using namespace Executor::PatternContainerStatementExecutor;
using namespace Types;

TEST_CASE("PatternContainerStatementExecutor<ClauseType::PatternWhile>::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setWhileControl(1, "x");
	pkb.setWhileControl(1, "y");
	pkb.setWhileControl(2, "z");
	pkb.setUses(1, "x");
	pkb.setUses(1, "y");
	pkb.setUses(2, "z");
	pkb.setUses(4, "a");
	pkb.populateComplexRelations();

	ClauseArgument syn_while = ClauseArgument(Declaration{Types::DesignEntity::While, "w"});
	ClauseArgument syn_var = ClauseArgument(Declaration{Types::DesignEntity::Variable, "v"});
	ClauseArgument a = ClauseArgument("a");
	ClauseArgument x = ClauseArgument("x");
	ClauseArgument y = ClauseArgument("y");
	ClauseArgument z = ClauseArgument("z");

	SECTION("Trivial: Name") {
		QueryResult result1 = executeTrivialName<ClauseType::PatternWhile>(store, x);
		QueryResult result2 = executeTrivialName<ClauseType::PatternWhile>(store, a);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard or Synonym") {
		QueryResult result1 = executeTrivialWildcardOrSynonym<ClauseType::PatternWhile>(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Non-Trivial: Name") {
		QueryResult result1 = executeName<ClauseType::PatternWhile>(store, syn_while, x);
		QueryResult result2 = executeName<ClauseType::PatternWhile>(store, syn_while, a);

		REQUIRE(result1.getSynonymResult("w") == vector<string>{"1"});
		REQUIRE(!result2.getResult());
	}

	SECTION("Non-Trivial: Wildcard") {
		QueryResult result1 = executeWildcard<ClauseType::PatternWhile>(store, syn_while);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("w");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("Non-Trivial: Synonym") {
		QueryResult result1 = executeSynonym<ClauseType::PatternWhile>(store, syn_while, syn_var);

		vector<string> expected_result_w = {"1", "1", "2"};
		vector<string> actual_result_w = result1.getSynonymResult("w");
		vector<string> expected_result_v = {"x", "y", "z"};
		vector<string> actual_result_v = result1.getSynonymResult("v");
		sort(actual_result_w.begin(), actual_result_w.end());
		sort(actual_result_v.begin(), actual_result_v.end());
		REQUIRE(actual_result_w == expected_result_w);
		REQUIRE(actual_result_v == expected_result_v);
	}
}