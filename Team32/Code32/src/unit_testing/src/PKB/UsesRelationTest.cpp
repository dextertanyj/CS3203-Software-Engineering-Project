#include "PKB/UsesRelation.h"

#include "TestUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::UsesRelation") {
	PKB::SVRelationStore<PKB::UsesRelation> store = PKB::SVRelationStore<PKB::UsesRelation>();
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(2, StmtType::IfStmt);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(4, StmtType::Call);
	shared_ptr<StmtInfo> s5 = TestUtilities::createStmtInfo(5, StmtType::Print);
	shared_ptr<StmtInfo> s6 = TestUtilities::createStmtInfo(6, StmtType::Read);

	SECTION("PKB::UsesRelation::validate One Var Test") {
		REQUIRE(PKB::UsesRelation::validate(&store, s1, "x"));
		REQUIRE(PKB::UsesRelation::validate(&store, s2, "x"));
		REQUIRE(PKB::UsesRelation::validate(&store, s3, "x"));
		REQUIRE(PKB::UsesRelation::validate(&store, s4, "x"));
		REQUIRE(PKB::UsesRelation::validate(&store, s5, "x"));
		REQUIRE_THROWS_AS(PKB::UsesRelation::validate(&store, s6, "x"), invalid_argument);
		store.set(s1, "x");
		store.set(s2, "x");
		store.set(s3, "x");
		store.set(s4, "x");
		store.set(s5, "x");
		REQUIRE(PKB::UsesRelation::validate(&store, s1, "x"));
		REQUIRE(PKB::UsesRelation::validate(&store, s1, "y"));
		REQUIRE(PKB::UsesRelation::validate(&store, s2, "y"));
		REQUIRE(PKB::UsesRelation::validate(&store, s3, "y"));
		REQUIRE(PKB::UsesRelation::validate(&store, s4, "y"));
		REQUIRE_FALSE(PKB::UsesRelation::validate(&store, s5, "y"));
	}

	SECTION("PKB::UsesRelation::validate Multiple Var Test") {
		unordered_set<VarRef> var_refs = unordered_set<VarRef>({"x", "y", "z"});
		REQUIRE(PKB::UsesRelation::validate(&store, s1, var_refs));
		REQUIRE(PKB::UsesRelation::validate(&store, s2, var_refs));
		REQUIRE(PKB::UsesRelation::validate(&store, s3, var_refs));
		REQUIRE(PKB::UsesRelation::validate(&store, s4, var_refs));
		REQUIRE_FALSE(PKB::UsesRelation::validate(&store, s5, var_refs));
		REQUIRE_THROWS_AS(PKB::UsesRelation::validate(&store, s6, var_refs), invalid_argument);
	}

	SECTION("PKB::UsesRelation::optimize Test") {
		PKB::Types::StatementStore statement_store = TestUtilities::generateStatementStore();
		PKB::StatementRelationStore<PKB::ParentRelation> parent_store = PKB::StatementRelationStore<PKB::ParentRelation>();
		unordered_set<VarRef> var_refs = unordered_set<VarRef>({"e", "f", "g"});

		shared_ptr<StmtInfo> p1 = statement_store.get(1);
		shared_ptr<StmtInfo> p2 = statement_store.get(2);
		shared_ptr<StmtInfo> p3 = statement_store.get(3);
		shared_ptr<StmtInfo> p4 = statement_store.get(4);
		shared_ptr<StmtInfo> p5 = statement_store.get(5);
		shared_ptr<StmtInfo> p6 = statement_store.get(6);
		store.set(p1, "x");
		store.set(p2, "y");
		store.set(p3, var_refs);
		store.set(p4, "a");
		store.set(p5, "b");
		parent_store.set(p1, p3);
		parent_store.set(p1, p4);
		parent_store.set(p2, p5);

		PKB::ParentRelation::optimize(parent_store);
		PKB::UsesRelation::optimize(statement_store, parent_store, store);
		VarRefSet expected_set_1 = {"x", "e", "a", "f", "g"};
		VarRefSet expected_set_2 = {"y", "b"};
		unordered_set<shared_ptr<StmtInfo>> expected_set_3 = {p1, p4};
		unordered_set<shared_ptr<StmtInfo>> expected_set_4 = {p2, p5};
		// Verify that the variables used by the if/while statements include variables in the if/while blocks
		CHECK(store.getByStmt(1) == expected_set_1);
		CHECK(store.getByStmt(2) == expected_set_2);
		CHECK(store.getByVar("a") == expected_set_3);
		CHECK(store.getByVar("b") == expected_set_4);
	}

	// If statement (index 2) is nested within while statement (index 1)
	SECTION("PKB::UsesRelation::optimize Nested Test") {
		PKB::Types::StatementStore statement_store = TestUtilities::generateStatementStore();
		PKB::StatementRelationStore<PKB::ParentRelation> parent_store = PKB::StatementRelationStore<PKB::ParentRelation>();
		unordered_set<VarRef> var_refs = unordered_set<VarRef>({"e", "f", "g"});

		shared_ptr<StmtInfo> p1 = statement_store.get(1);
		shared_ptr<StmtInfo> p2 = statement_store.get(2);
		shared_ptr<StmtInfo> p3 = statement_store.get(3);
		shared_ptr<StmtInfo> p4 = statement_store.get(4);
		shared_ptr<StmtInfo> p5 = statement_store.get(5);
		shared_ptr<StmtInfo> p6 = statement_store.get(6);
		store.set(p1, "x");
		store.set(p2, "y");
		store.set(p3, var_refs);
		store.set(p4, "a");
		store.set(p5, "b");
		parent_store.set(p1, p2);
		parent_store.set(p1, p5);
		parent_store.set(p2, p4);
		parent_store.set(p2, p3);

		PKB::ParentRelation::optimize(parent_store);
		PKB::UsesRelation::optimize(statement_store, parent_store, store);

		VarRefSet expected_set_1 = {"x", "e", "a", "y", "b", "f", "g"};
		VarRefSet expected_set_2 = {"y", "a", "e", "f", "g"};
		unordered_set<shared_ptr<StmtInfo>> expected_set_3 = {p1, p2, p4};
		unordered_set<shared_ptr<StmtInfo>> expected_set_4 = {p1, p5};
		CHECK(store.getByStmt(1) == expected_set_1);
		CHECK(store.getByStmt(2) == expected_set_2);
		CHECK(store.getByVar("a") == expected_set_3);
		CHECK(store.getByVar("b") == expected_set_4);
	}
}
