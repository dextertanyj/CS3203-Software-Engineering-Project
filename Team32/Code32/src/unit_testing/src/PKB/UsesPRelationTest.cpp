#include "PKB/UsesPRelation.h"

#include "MockUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::UsesPRelation::optimize Test") {
	PKB::ProcedureStore procedure_store = PKB::ProcedureStore();
	PKB::PVRelationStore<PKB::UsesPRelation> store = PKB::PVRelationStore<PKB::UsesPRelation>();
	PKB::SVRelationStore<PKB::UsesSRelation> uses_s_store = PKB::SVRelationStore<PKB::UsesSRelation>();

	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::Call);
	shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(4, StmtType::Call);
	shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(5, StmtType::Print);
	ProcRef p1 = "one";
	ProcRef p2 = "two";
	VarRef x = "x";
	VarRef y = "y";
	VarRefSet v = {"a", "x"};

	procedure_store.insert(p1, {s1, s2});
	procedure_store.insert(p2, {s3, s4, s5});
	uses_s_store.set(s1, v);
	uses_s_store.set(s3, x);
	uses_s_store.set(s5, y);

	PKB::UsesPRelation::optimize(procedure_store, store, uses_s_store);
	CHECK(store.getByProc(p1) == VarRefSet{"a", "x"});
	CHECK(store.getByProc(p2) == VarRefSet{"x", "y"});
	CHECK(store.getByVar("x") == ProcRefSet{p1, p2});
	CHECK(store.getByVar("a") == ProcRefSet{p1});
};