#include "PKB/CallsRelation.h"

#include <memory>
#include <unordered_set>

#include "TestUtilities.h"
#include "catch_tools.h"

typedef unordered_set<shared_ptr<PKB::ProcedureInfo>> ProcedureInfoPtrSet;

TEST_CASE("PKB::CallsRelation Tests") {
	ProcRef p_1 = "procedure";
	ProcRef p_2 = "main";
	ProcRef p_3 = "hello";
	ProcRef p_4 = "world";
	shared_ptr<PKB::ProcedureInfo> p_1_ptr = TestUtilities::createProcedureInfo(p_1);
	shared_ptr<PKB::ProcedureInfo> p_2_ptr = TestUtilities::createProcedureInfo(p_2);
	shared_ptr<PKB::ProcedureInfo> p_3_ptr = TestUtilities::createProcedureInfo(p_3);
	shared_ptr<PKB::ProcedureInfo> p_4_ptr = TestUtilities::createProcedureInfo(p_4);
	PKB::CallsRelation relation = PKB::CallsRelation(p_1_ptr);

	SECTION("Self") { REQUIRE_EQUALS(relation.getSelf(), p_1_ptr); }

	SECTION("InsertForward") {
		relation.insertForward(p_2_ptr);
		relation.insertForward(p_3_ptr);
		REQUIRE_EQUALS(relation.getForward(), ProcedureInfoPtrSet({p_2_ptr, p_3_ptr}));
	}

	SECTION("InsertReverse") {
		relation.insertReverse(p_2_ptr);
		relation.insertReverse(p_3_ptr);
		REQUIRE_EQUALS(relation.getReverse(), ProcedureInfoPtrSet({p_2_ptr, p_3_ptr}));
	}

	SECTION("Insert... Repeated") {
		relation.insertForward(p_2_ptr);
		relation.insertForward(p_2_ptr);
		REQUIRE_EQUALS(relation.getForward(), ProcedureInfoPtrSet({p_2_ptr}));
	}

	SECTION("AppendForwardTransitive") {
		relation.appendForwardTransitive({p_2_ptr, p_3_ptr});
		relation.appendForwardTransitive({p_4_ptr});
		REQUIRE_EQUALS(relation.getForwardTransitive(), ProcedureInfoPtrSet({p_2_ptr, p_3_ptr, p_4_ptr}));
	}

	SECTION("AppendReverseTransitive") {
		relation.appendReverseTransitive({p_2_ptr, p_3_ptr});
		relation.appendReverseTransitive({p_4_ptr});
		REQUIRE_EQUALS(relation.getReverseTransitive(), ProcedureInfoPtrSet({p_2_ptr, p_3_ptr, p_4_ptr}));
	}

	SECTION("Append...Transitive Repeated") {
		relation.appendForwardTransitive({p_2_ptr, p_3_ptr});
		relation.appendForwardTransitive({p_3_ptr, p_4_ptr});
		REQUIRE_EQUALS(relation.getForwardTransitive(), ProcedureInfoPtrSet({p_2_ptr, p_3_ptr, p_4_ptr}));
	}

	SECTION("Recursion Detection") {
		REQUIRE_THROWS_AS(relation.insertForward(p_1_ptr), PKB::CallGraphException);
		REQUIRE_THROWS_AS(relation.insertReverse(p_1_ptr), PKB::CallGraphException);
		REQUIRE_THROWS_AS(relation.appendForwardTransitive({p_1_ptr, p_2_ptr}), PKB::CallGraphException);
		REQUIRE_THROWS_AS(relation.appendReverseTransitive({p_1_ptr, p_2_ptr}), PKB::CallGraphException);
	}
}

TEST_CASE("PKB::TransitiveRelationStore<...,...,Call...>::optimize Test") {
	PKB::Types::StatementStore statements;
	PKB::Types::ProcedureStore procedures;
	PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation> store;

	SECTION("Chain Test") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3;
		ProcRef p_1 = "p1", p_2 = "p2", p_3 = "p3";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Read);
		procedures.insert(p_1, {statements.get(s_1)});
		procedures.insert(p_2, {statements.get(s_2)});
		procedures.insert(p_3, {statements.get(s_3)});

		store.set(procedures.get(p_1), procedures.get(p_2));
		store.set(procedures.get(p_2), procedures.get(p_3));

		REQUIRE_NOTHROW(store.optimize());

		REQUIRE_EQUALS(store.getReverse(p_1), ProcedureInfoPtrSet({procedures.get(p_2)}));
		REQUIRE_EQUALS(store.getForward(p_3), ProcedureInfoPtrSet({procedures.get(p_2)}));

		REQUIRE(store.getForwardTransitive(p_1).empty());
		REQUIRE_EQUALS(store.getReverseTransitive(p_1), ProcedureInfoPtrSet({procedures.get(p_2), procedures.get(p_3)}));
		REQUIRE_EQUALS(store.getReverseTransitive(p_2), ProcedureInfoPtrSet({procedures.get(p_3)}));
		REQUIRE_EQUALS(store.getForwardTransitive(p_2), ProcedureInfoPtrSet({procedures.get(p_1)}));
		REQUIRE_EQUALS(store.getForwardTransitive(p_3), ProcedureInfoPtrSet({procedures.get(p_1), procedures.get(p_2)}));
		REQUIRE(store.getReverseTransitive(p_3).empty());
	}

	SECTION("Star Graph Test") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3, s_4 = 4, s_5 = 5;
		ProcRef p_1 = "p1", p_2 = "p2", p_3 = "p3", p_4 = "p4", p_5 = "p5";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Call);
		statements.insert(s_4, StmtType::Print);
		statements.insert(s_5, StmtType::Read);
		procedures.insert(p_1, {statements.get(s_1)});
		procedures.insert(p_2, {statements.get(s_2)});
		procedures.insert(p_3, {statements.get(s_3)});
		procedures.insert(p_4, {statements.get(s_4)});
		procedures.insert(p_5, {statements.get(s_5)});

		store.set(procedures.get(p_1), procedures.get(p_3));
		store.set(procedures.get(p_2), procedures.get(p_3));
		store.set(procedures.get(p_3), procedures.get(p_4));
		store.set(procedures.get(p_3), procedures.get(p_5));

		REQUIRE_NOTHROW(store.optimize());

		REQUIRE(store.getForwardTransitive(p_1).empty());
		REQUIRE(store.getForwardTransitive(p_2).empty());
		REQUIRE_EQUALS(store.getReverseTransitive(p_1),
		               ProcedureInfoPtrSet({procedures.get(p_3), procedures.get(p_4), procedures.get(p_5)}));
		REQUIRE_EQUALS(store.getReverseTransitive(p_2),
		               ProcedureInfoPtrSet({procedures.get(p_3), procedures.get(p_4), procedures.get(p_5)}));
		REQUIRE_EQUALS(store.getForwardTransitive(p_3), ProcedureInfoPtrSet({procedures.get(p_1), procedures.get(p_2)}));
		REQUIRE_EQUALS(store.getReverseTransitive(p_3), ProcedureInfoPtrSet({procedures.get(p_4), procedures.get(p_5)}));
		REQUIRE_EQUALS(store.getForwardTransitive(p_4),
		               ProcedureInfoPtrSet({procedures.get(p_1), procedures.get(p_2), procedures.get(p_3)}));
		REQUIRE_EQUALS(store.getForwardTransitive(p_5),
		               ProcedureInfoPtrSet({procedures.get(p_1), procedures.get(p_2), procedures.get(p_3)}));
		REQUIRE(store.getReverseTransitive(p_4).empty());
		REQUIRE(store.getReverseTransitive(p_5).empty());
	}

	SECTION("Loop Test") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3, s_4 = 4;
		ProcRef p_1 = "p1", p_2 = "p2", p_3 = "p3", p_4 = "p4";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Call);
		statements.insert(s_4, StmtType::Call);
		procedures.insert(p_1, {statements.get(s_1)});
		procedures.insert(p_2, {statements.get(s_2)});
		procedures.insert(p_3, {statements.get(s_3)});
		procedures.insert(p_4, {statements.get(s_4)});

		store.set(procedures.get(p_1), procedures.get(p_2));
		store.set(procedures.get(p_2), procedures.get(p_3));
		store.set(procedures.get(p_3), procedures.get(p_4));
		store.set(procedures.get(p_4), procedures.get(p_2));

		REQUIRE_THROWS_AS(store.optimize(), PKB::CallGraphException);
	}

	SECTION("Pure Loop Test") {
		StmtRef s_1 = 1, s_2 = 2, s_3 = 3, s_4 = 4;
		ProcRef p_1 = "p1", p_2 = "p2", p_3 = "p3", p_4 = "p4";
		statements.insert(s_1, StmtType::Call);
		statements.insert(s_2, StmtType::Call);
		statements.insert(s_3, StmtType::Call);
		statements.insert(s_4, StmtType::Call);
		procedures.insert(p_1, {statements.get(s_1)});
		procedures.insert(p_2, {statements.get(s_2)});
		procedures.insert(p_3, {statements.get(s_3)});
		procedures.insert(p_4, {statements.get(s_4)});

		store.set(procedures.get(p_1), procedures.get(p_2));
		store.set(procedures.get(p_2), procedures.get(p_3));
		store.set(procedures.get(p_3), procedures.get(p_4));
		store.set(procedures.get(p_4), procedures.get(p_1));

		REQUIRE_NOTHROW(store.optimize());
	}
}