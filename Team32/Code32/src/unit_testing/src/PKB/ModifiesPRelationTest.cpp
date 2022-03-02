#include "PKB/ModifiesPRelation.h"

#include "MockUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::ModifiesPRelation::optimize Test") {
	PKB::ProcedureStore procedure_store = PKB::ProcedureStore();
	PKB::PVRelationStore<PKB::ModifiesPRelation> store = PKB::PVRelationStore<PKB::ModifiesPRelation>();
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_s_store = PKB::SVRelationStore<PKB::ModifiesSRelation>();

	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::Call);
	shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(4, StmtType::Call);
	shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(5, StmtType::Read);
	ProcRef p1 = "one";
	ProcRef p2 = "two";
	VarRef x = "x";
	VarRef y = "y";

	procedure_store.insert(p1, {s1, s2});
	procedure_store.insert(p2, {s3, s4, s5});
	modifies_s_store.set(s1, y);
	modifies_s_store.set(s3, x);
	modifies_s_store.set(s5, y);

	PKB::ModifiesPRelation::optimize(procedure_store, store, modifies_s_store);
	CHECK(store.getByProc(p1) == VarRefSet{"y"});
	CHECK(store.getByProc(p2) == VarRefSet{"x", "y"});
	CHECK(store.getByVar("y") == ProcRefSet{p1, p2});
	CHECK(store.getByVar("x") == ProcRefSet{p2});
};