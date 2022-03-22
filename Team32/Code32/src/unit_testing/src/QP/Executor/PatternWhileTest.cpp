#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor;

TEST_CASE("PatternContainerStatementExecutor<ClauseType::PatternWhile>::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
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

	ReferenceArgument syn_while = ReferenceArgument(Declaration{QP::Types::DesignEntity::While, "w"});
	ReferenceArgument syn_var = ReferenceArgument(Declaration{QP::Types::DesignEntity::Variable, "v"});
	ReferenceArgument a = ReferenceArgument("a");
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument z = ReferenceArgument("z");

	SECTION("Trivial: Name") {
		QP::QueryResult result1 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialName(store, x);
		QP::QueryResult result2 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialName(store, a);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard or Synonym") {
		QP::QueryResult result1 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialWildcardOrSynonym(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Non-Trivial: Name") {
		QP::QueryResult result1 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeName(store, syn_while, x);
		QP::QueryResult result2 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeName(store, syn_while, a);

		REQUIRE(result1.getSynonymResult("w") == vector<string>{"1"});
		REQUIRE(!result2.getResult());
	}

	SECTION("Non-Trivial: Wildcard") {
		QP::QueryResult result1 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeWildcard(store, syn_while);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("w");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("Non-Trivial: Synonym") {
		QP::QueryResult result1 = PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeSynonym(store, syn_while, syn_var);

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