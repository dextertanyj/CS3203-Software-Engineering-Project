#include "PKB/Modifies.h"

#include "MockUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::Modifies") {
    SVRelationStore<Modifies> store = SVRelationStore<Modifies>();
    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::IfStmt);
    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(3, StmtType::Assign);
    shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(4, StmtType::Call);
    shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(5, StmtType::Print);
    shared_ptr<StmtInfo> s6 = MockUtilities::createStmtInfo(6, StmtType::Read);

    SECTION("PKB::Modifies::validate One Var Test") {
        REQUIRE(Modifies::validate(&store, s1, "x"));
        REQUIRE(Modifies::validate(&store, s2, "x"));
        REQUIRE(Modifies::validate(&store, s3, "x"));
        REQUIRE(Modifies::validate(&store, s4, "x"));
        REQUIRE_THROWS_AS(Modifies::validate(&store, s5, "x"), invalid_argument);
        REQUIRE(Modifies::validate(&store, s6, "x"));
        store.set(s1, "x");
        store.set(s2, "x");
        store.set(s3, "x");
        store.set(s4, "x");
        store.set(s6, "x");
        REQUIRE(Modifies::validate(&store, s1, "y"));
        REQUIRE(Modifies::validate(&store, s2, "y"));
        REQUIRE_FALSE(Modifies::validate(&store, s3, "y"));
        REQUIRE(Modifies::validate(&store, s4, "y"));
        REQUIRE_FALSE(Modifies::validate(&store, s6, "y"));
    }

    SECTION("PKB::Modifies::validate Multiple Var Test") {
        unordered_set<VarRef> varRefs = unordered_set<VarRef>({"x", "y", "z"});
        REQUIRE(Modifies::validate(&store, s1, varRefs));
        REQUIRE(Modifies::validate(&store, s2, varRefs));
        REQUIRE_FALSE(Modifies::validate(&store, s3, varRefs));
        REQUIRE(Modifies::validate(&store, s4, varRefs));
        REQUIRE_THROWS_AS(Modifies::validate(&store, s5, varRefs), invalid_argument);
        REQUIRE_FALSE(Modifies::validate(&store, s6, varRefs));
    }

//    SECTION("PKB::Modifies::optimize Test") {
//        StatementStore statement_store = StatementStore();
//        StatementRelationStore<ParentPKB> parent_store = StatementRelationStore<ParentPKB>();
//
//        statement_store.insert(1, StmtType::WhileStmt);
//        statement_store.insert(2, StmtType::IfStmt);
//        statement_store.insert(3, StmtType::Assign);
//        statement_store.insert(4, StmtType::Call);
//        statement_store.insert(5, StmtType::Print);
//        statement_store.insert(6, StmtType::Read);
//        shared_ptr<StmtInfo> s1 = statement_store.get(1);
//        shared_ptr<StmtInfo> s2 = statement_store.get(2);
//        shared_ptr<StmtInfo> s3 = statement_store.get(3);
//        shared_ptr<StmtInfo> s4 = statement_store.get(4);
//        shared_ptr<StmtInfo> s5 = statement_store.get(5);
//        shared_ptr<StmtInfo> s6 = statement_store.get(6);
//        store.set(s1, "x");
//        store.set(s2, "y");
//        store.set(s3, "z");
//        store.set(s4, "a");
//        store.set(s6, "b");
//        parent_store.set(s1, s3);
//        parent_store.set(s1, s4);
//        parent_store.set(s2, s6);
//
//        Modifies::optimize(statement_store, parent_store, store);
//        VarRefSet expected_set_1 = { "x", "z", "a" };
//        VarRefSet expected_set_2 = { "y", "b" };
//        // Verify that the variables modified by the if/while statements include variables in the if/while blocks
//        CHECK(store.getByStmt(1) == expected_set_1);
//        CHECK(store.getByStmt(2) == expected_set_2);
//    }

}
