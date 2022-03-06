#include "PKB/ModifiesSRelation.h"

#include "TestUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::ModifiesSRelation Test") {
	PKB::SVRelationStore<PKB::ModifiesSRelation> store = PKB::SVRelationStore<PKB::ModifiesSRelation>();
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(2, StmtType::IfStmt);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(4, StmtType::Call);
	shared_ptr<StmtInfo> s5 = TestUtilities::createStmtInfo(5, StmtType::Read);
	shared_ptr<StmtInfo> s6 = TestUtilities::createStmtInfo(6, StmtType::Read);
	shared_ptr<StmtInfo> s7 = TestUtilities::createStmtInfo(7, StmtType::Call);
	shared_ptr<StmtInfo> s8 = TestUtilities::createStmtInfo(8, StmtType::Assign);
	shared_ptr<StmtInfo> s9 = TestUtilities::createStmtInfo(9, StmtType::Print);

	SECTION("PKB::ModifiesSRelation::validate One Var Test") {
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s1, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s2, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s3, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s4, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s5, "x"));
		REQUIRE(PKB::ModifiesSRelation::validate(&store, s6, "x"));

		// Print statements cannot modify a variable.
		REQUIRE_THROWS_AS(PKB::ModifiesSRelation::validate(&store, s9, "x"), invalid_argument);

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
		REQUIRE_THROWS_AS(PKB::ModifiesSRelation::validate(&store, s9, var_refs), invalid_argument);
	}

	SECTION("PKB::ModifiesSRelation::optimize Test") {
		/* Nested conditional statements:
		 * main_proc:
		 * s1 ---
		 * 	 \   \
		 * 	  s2  s5
		 * 	   \
		 * 	    s3 -- s4
		 *
		 * sub_proc_1 (Called by s4):
		 * s6 -- s7
		 *
		 * sub_proc_2 (called by s7):
		 * s8
		 */
		// Initialise necessary stores for optimize.
		PKB::Types::ProcedureStore procedure_store = PKB::Types::ProcedureStore();
		PKB::CallStatementStore call_statement_store = PKB::CallStatementStore();
		PKB::TopologicalSort<PKB::ProcedureInfo> call_graph = PKB::TopologicalSort<PKB::ProcedureInfo>();
		PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation> transitive_rel_store =
			PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation>();

		ProcRef main_proc = "main";
		ProcRef sub_proc_1 = "sub_1";
		ProcRef sub_proc_2 = "sub_2";
		call_statement_store.set(s4, sub_proc_1);
		call_statement_store.set(s7, sub_proc_2);
		procedure_store.insert(main_proc, {s1, s2, s3, s4, s5});
		procedure_store.insert(sub_proc_1, {s6, s7});
		procedure_store.insert(sub_proc_2, {s8});
		transitive_rel_store.set(procedure_store.get(main_proc), procedure_store.get(sub_proc_1));
		transitive_rel_store.set(procedure_store.get(sub_proc_1), procedure_store.get(sub_proc_2));
		call_graph.sort(procedure_store, transitive_rel_store);

		PKB::Types::ParentStore parent_store = PKB::Types::ParentStore();

		store.set(s3, "z");
		store.set(s5, "a");
		store.set(s6, "b");
		store.set(s8, "c");
		parent_store.set(s1, s2);
		parent_store.set(s1, s5);
		parent_store.set(s2, s4);
		parent_store.set(s2, s3);

		parent_store.optimize();
		PKB::ModifiesSRelation::optimize(parent_store, call_statement_store, procedure_store, call_graph, store);

		/* If optimize works correctly:
		 * 1) var 'c' from s8 in sub_proc_2 propagates up to s7, s4, s2, and s1.
		 * 2) var 'b' from s6 in sub_proc_1 propagates to s4, s2 and s1.
		 * 3) var 'a' from s5 propagates up to s1,
		 * 4) var 'z' from s3 propagates up to s2 and s1.
		*/
		VarRefSet expected_set_s1 = {"z", "a", "b", "c"};
		VarRefSet expected_set_s2 = {"z", "b", "c"};
		VarRefSet expected_set_s3 = {"z"};
		VarRefSet expected_set_s4 = {"b", "c"};
		VarRefSet expected_set_s5 = {"a"};
		VarRefSet expected_set_s6 = {"b"};
		VarRefSet expected_set_s7 = {"c"};
		VarRefSet expected_set_s8 = {"c"};

		StmtInfoPtrSet expected_set_var_z = {s1, s2, s3};
		StmtInfoPtrSet expected_set_var_a = {s1, s5};
		StmtInfoPtrSet expected_set_var_b = {s1, s2, s4, s6};
		StmtInfoPtrSet expected_set_var_c = {s1, s2, s4, s7, s8};

		CHECK(store.getByStmt(1) == expected_set_s1);
		CHECK(store.getByStmt(2) == expected_set_s2);
		CHECK(store.getByStmt(3) == expected_set_s3);
		CHECK(store.getByStmt(4) == expected_set_s4);
		CHECK(store.getByStmt(5) == expected_set_s5);
		CHECK(store.getByStmt(6) == expected_set_s6);
		CHECK(store.getByStmt(7) == expected_set_s7);
		CHECK(store.getByStmt(8) == expected_set_s8);

		CHECK(store.getByVar("z") == expected_set_var_z);
		CHECK(store.getByVar("a") == expected_set_var_a);
		CHECK(store.getByVar("b") == expected_set_var_b);
		CHECK(store.getByVar("c") == expected_set_var_c);
	}
}
