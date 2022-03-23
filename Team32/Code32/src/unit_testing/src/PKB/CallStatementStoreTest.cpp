#include "PKB/CallStatementStore.h"

#include <memory>
#include <unordered_set>

#include "TestUtilities.h"
#include "catch_tools.h"

typedef unordered_set<shared_ptr<PKB::ProcedureInfo>> ProcedureInfoPtrSet;

TEST_CASE("PKB::CallStatementStore::set Test") {
	PKB::CallStatementStore store;
	ProcRef p_1 = "main";
	ProcRef p_2 = "method";

	SECTION("Valid") {
		shared_ptr<StmtInfo> call = TestUtilities::createStmtInfo(1, StmtType::Call);
		store.set(call, p_1);
		REQUIRE_EQUALS(store.getProcedure(call), p_1);
	}

	SECTION("Incorrect Statement Type") {
		shared_ptr<StmtInfo> print = TestUtilities::createStmtInfo(1, StmtType::Print);
		REQUIRE_THROWS_AS(store.set(print, p_1), logic_error);
	}

	SECTION("Repeated Set") {
		shared_ptr<StmtInfo> call = TestUtilities::createStmtInfo(1, StmtType::Call);
		store.set(call, p_1);
		REQUIRE_THROWS_AS(store.set(call, p_2), logic_error);
	}
}

TEST_CASE("PKB::CallStatementStore::get Test") {
	PKB::CallStatementStore store;
	ProcRef p_1 = "main";

	SECTION("By Statement Number") {
		shared_ptr<StmtInfo> call = TestUtilities::createStmtInfo(1, StmtType::Call);
		store.set(call, p_1);
		REQUIRE_EQUALS(store.getProcedure(1), p_1);
	}

	SECTION("Unset Statement") {
		shared_ptr<StmtInfo> call = TestUtilities::createStmtInfo(1, StmtType::Call);
		REQUIRE_THROWS_AS(store.getProcedure(call), logic_error);
	}
}

TEST_CASE("PKB::CallStatementStore::populate Test") {
	PKB::CallStatementStore store;
	PKB::Types::StatementStore statements;
	PKB::Types::ProcedureStore procedures;
	PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation> call_store;

	SECTION("Single Call") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3;
		ProcRef p_1 = "p1", p_2 = "p_2";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Print);
		statements.insert(s_3, StmtType::Read);
		procedures.insert(p_1, {statements.get(s_1), statements.get(s_2)});
		procedures.insert(p_2, {statements.get(s_3)});
		store.set(statements.get(s_1), p_2);

		store.populate(procedures, call_store);

		REQUIRE_EQUALS(call_store.getReverse(p_1), ProcedureInfoPtrSet({procedures.get(p_2)}));
		REQUIRE(call_store.getForward(p_1).empty());
		REQUIRE_EQUALS(call_store.getForward(p_2), ProcedureInfoPtrSet({procedures.get(p_1)}));
		REQUIRE(call_store.getReverse(p_2).empty());
	}

	SECTION("Multiple Calls") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3, s_4 = 4;
		ProcRef p_1 = "p1", p_2 = "p_2", p_3 = "p_3";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Read);
		statements.insert(s_4, StmtType::Print);
		procedures.insert(p_1, {statements.get(s_1), statements.get(s_2)});
		procedures.insert(p_2, {statements.get(s_3)});
		procedures.insert(p_3, {statements.get(s_4)});
		store.set(statements.get(s_2), p_3);
		store.set(statements.get(s_1), p_2);

		store.populate(procedures, call_store);

		REQUIRE_EQUALS(call_store.getReverse(p_1), ProcedureInfoPtrSet({procedures.get(p_2), procedures.get(p_3)}));
		REQUIRE(call_store.getForward(p_1).empty());
		REQUIRE_EQUALS(call_store.getForward(p_2), ProcedureInfoPtrSet({procedures.get(p_1)}));
		REQUIRE(call_store.getReverse(p_2).empty());
		REQUIRE_EQUALS(call_store.getForward(p_3), ProcedureInfoPtrSet({procedures.get(p_1)}));
		REQUIRE(call_store.getReverse(p_3).empty());
	}

	SECTION("Chain Calls") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3;
		ProcRef p_1 = "p1", p_2 = "p_2", p_3 = "p_3";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Print);
		procedures.insert(p_1, {statements.get(s_1)});
		procedures.insert(p_2, {statements.get(s_2)});
		procedures.insert(p_3, {statements.get(s_3)});
		store.set(statements.get(s_2), p_3);
		store.set(statements.get(s_1), p_2);

		store.populate(procedures, call_store);

		REQUIRE_EQUALS(call_store.getReverse(p_1), ProcedureInfoPtrSet({procedures.get(p_2)}));
		REQUIRE(call_store.getForward(p_1).empty());
		REQUIRE_EQUALS(call_store.getForward(p_2), ProcedureInfoPtrSet({procedures.get(p_1)}));
		REQUIRE_EQUALS(call_store.getReverse(p_2), ProcedureInfoPtrSet({procedures.get(p_3)}));
		REQUIRE_EQUALS(call_store.getForward(p_3), ProcedureInfoPtrSet({procedures.get(p_2)}));
		REQUIRE(call_store.getReverse(p_3).empty());
	}

	SECTION("Missing Calls") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3, s_4 = 4;
		ProcRef p_1 = "p1", p_2 = "p_2";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Read);
		statements.insert(s_4, StmtType::Print);
		procedures.insert(p_1, {statements.get(s_1), statements.get(s_2)});
		procedures.insert(p_2, {statements.get(s_3)});
		store.set(statements.get(s_1), p_2);

		REQUIRE_THROWS_AS(store.populate(procedures, call_store), logic_error);
	}

	SECTION("Missing Procedure") {
		StmtRef s_1 = 1;
		ProcRef p_1 = "p1", p_2 = "p_2";
		statements.insert(s_1, StmtType::Call);
		procedures.insert(p_1, {statements.get(s_1)});
		store.set(statements.get(s_1), p_2);

		REQUIRE_THROWS_AS(store.populate(procedures, call_store), logic_error);
	}
}
