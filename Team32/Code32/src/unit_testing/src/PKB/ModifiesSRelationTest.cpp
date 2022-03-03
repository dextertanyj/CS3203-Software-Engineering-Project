#include "PKB/ModifiesSRelation.h"

#include "TestUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::ModifiesSRelation Test") {
	/* Nested conditional statements:
	 * main_proc:
	 * 1 --
	 * 	\   \
	 * 	 2   5
	 * 	  \
	 * 	   3 --- 4
	 *
	 * sub_proc (Called by s4):
	 * 6
	 * */
	PKB::SVRelationStore<PKB::ModifiesSRelation> store = PKB::SVRelationStore<PKB::ModifiesSRelation>();
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(2, StmtType::IfStmt);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(4, StmtType::Call);
	shared_ptr<StmtInfo> s5 = TestUtilities::createStmtInfo(5, StmtType::Read);
	shared_ptr<StmtInfo> s6 = TestUtilities::createStmtInfo(6, StmtType::Read);
	shared_ptr<StmtInfo> s7 = TestUtilities::createStmtInfo(6, StmtType::Print);

	PKB::Types::ProcedureStore procedure_store = PKB::Types::ProcedureStore();
	PKB::CallStatementStore call_statement_store = PKB::CallStatementStore();
	PKB::TopologicalSort<PKB::ProcedureInfo> call_graph = PKB::TopologicalSort<PKB::ProcedureInfo>();
	PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation> transitive_rel_store =
		PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation>();
	ProcRef main_proc = "main";
	ProcRef sub_proc = "sub";
	call_statement_store.set(s4, sub_proc);
	procedure_store.insert(main_proc, {s1, s2, s3, s4, s5});
	procedure_store.insert(sub_proc, {s6});
	transitive_rel_store.set(procedure_store.get(main_proc), procedure_store.get(sub_proc));
	call_graph.sort(procedure_store, transitive_rel_store);

	SECTION("PKB::ModifiesSRelation::validate One Var Test") {
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s1, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s2, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s3, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s4, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s5, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s6, "x"));

		// Print statements cannot modify a variable.
		REQUIRE_THROWS_AS(PKB::ModifiesSRelation::validate(&store, s7, "x"), invalid_argument);

		store.set(s1, "x");
		store.set(s2, "x");
		store.set(s3, "x");
		store.set(s4, "x");
		store.set(s6, "x");
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s1, "y"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s2, "y"));
		REQUIRE_FALSE(PKB::ModifiesSRelation::validate(&store, s3, "y"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s4, "y"));
		REQUIRE_FALSE(PKB::ModifiesSRelation::validate(&store, s6, "y"));
	}

	SECTION("PKB::ModifiesSRelation::validate Multiple Var Test") {
		VarRefSet var_refs = {"x", "y", "z"};
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s1, var_refs));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s2, var_refs));
		REQUIRE_FALSE(PKB::ModifiesSRelation::validate(&store, s3, var_refs));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s4, var_refs));
		REQUIRE_FALSE(PKB::ModifiesSRelation::validate(&store, s5, var_refs));
		REQUIRE_FALSE(PKB::ModifiesSRelation::validate(&store, s6, var_refs));
		REQUIRE_THROWS_AS(PKB::ModifiesSRelation::validate(&store, s7, var_refs), invalid_argument);
	}

	// TODO: Rewrite optimize to include call stores.
	SECTION("PKB::ModifiesSRelation::optimize Test") {
		PKB::StatementRelationStore<PKB::ParentRelation> parent_store = PKB::StatementRelationStore<PKB::ParentRelation>();

		store.set(s3, "z");
		store.set(s5, "a");
		store.set(s6, "b");
		parent_store.set(s1, s2);
		parent_store.set(s1, s5);
		parent_store.set(s2, s4);
		parent_store.set(s2, s3);

		PKB::ParentRelation::optimize(parent_store);
		PKB::ModifiesSRelation::optimize(parent_store, call_statement_store, procedure_store, call_graph, store);

		// If optimize works correctly, var "b" from s6 in sub_proc should
		// propagate to s4 in main_proc, which would propagate up to s2 and s1.
		// Var "a" from s5 should propagate up to s1, and var "z" from s3 should
		// propagate up to s2 and s1.

		VarRefSet expected_set_s1 = {"z", "a", "b"};
		VarRefSet expected_set_s2 = {"z", "b"};
		VarRefSet expected_set_s3 = {"z"};
		VarRefSet expected_set_s4 = {"b"};
		VarRefSet expected_set_s5 = {"a"};
		VarRefSet expected_set_s6 = {"b"};

		StmtInfoPtrSet expected_set_var_z = {s1, s2, s3};
		StmtInfoPtrSet expected_set_var_a = {s1, s5};
		StmtInfoPtrSet expected_set_var_b = {s1, s2, s4, s6};

		CHECK(store.getByStmt(1) == expected_set_s1);
		CHECK(store.getByStmt(2) == expected_set_s2);
		CHECK(store.getByStmt(3) == expected_set_s3);
		CHECK(store.getByStmt(4) == expected_set_s4);
		CHECK(store.getByStmt(5) == expected_set_s5);
		CHECK(store.getByStmt(6) == expected_set_s6);

		CHECK(store.getByVar("z") == expected_set_var_z);
		CHECK(store.getByVar("a") == expected_set_var_a);
		CHECK(store.getByVar("b") == expected_set_var_b);

	}
	/*
	SECTION("PKB::ModifiesSRelation::optimize Conditional Test") {
		PKB::Types::StatementStore statement_store = TestUtilities::generateStatementStore();
		PKB::StatementRelationStore<PKB::ParentRelation> parent_store = PKB::StatementRelationStore<PKB::ParentRelation>();

		shared_ptr<StmtInfo> p1 = statement_store.get(1);
		shared_ptr<StmtInfo> p2 = statement_store.get(2);
		shared_ptr<StmtInfo> p3 = statement_store.get(3);
		shared_ptr<StmtInfo> p4 = statement_store.get(4);
		shared_ptr<StmtInfo> p5 = statement_store.get(5);
		shared_ptr<StmtInfo> p6 = statement_store.get(6);
//		store.set(p1, "x");
//		store.set(p2, "y");
		store.set(p3, "z");
		store.set(p4, "a");
		store.set(p6, "b");
		parent_store.set(p1, p2);
		parent_store.set(p1, p6);
		parent_store.set(p2, p4);
		parent_store.set(p2, p3);

		PKB::ParentRelation::optimize(parent_store);
		PKB::ModifiesSRelation::optimize(parent_store, call_statement_store, procedure_store, call_graph, store);

		VarRefSet expected_set_1 = {"x", "z", "a", "y", "b"};
		VarRefSet expected_set_2 = {"y", "a", "z"};
		StmtInfoPtrSet expected_set_3 = {p1, p2, p4};
		StmtInfoPtrSet expected_set_4 = {p1, p6};
		CHECK(store.getByStmt(1) == expected_set_1);
		CHECK(store.getByStmt(2) == expected_set_2);
		CHECK(store.getByVar("a") == expected_set_3);
		CHECK(store.getByVar("b") == expected_set_4);
	}
	*/
}
