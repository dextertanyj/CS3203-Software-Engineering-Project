#include "PKB/Modifies.h"

#include "MockUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::Modifies::validate One Var Test") {
    SVRelationStore<Modifies> store = SVRelationStore<Modifies>();
    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(1, StmtType::IfStmt);
    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(1, StmtType::Assign);
    shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(1, StmtType::Call);
    shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(1, StmtType::Print);
    shared_ptr<StmtInfo> s6 = MockUtilities::createStmtInfo(1, StmtType::Read);
    REQUIRE(Modifies::validate(&store, s1, "x"));
    REQUIRE(Modifies::validate(&store, s2, "x"));
    REQUIRE(Modifies::validate(&store, s3, "x"));
    REQUIRE(Modifies::validate(&store, s4, "x"));
    REQUIRE_THROWS_AS(Modifies::validate(&store, s5, "x"), std::exception);
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

TEST_CASE("PKB::Modifies::validate Multiple Var Test") {
    SVRelationStore<Modifies> store = SVRelationStore<Modifies>();
    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(1, StmtType::IfStmt);
    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(1, StmtType::Assign);
    shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(1, StmtType::Call);
    shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(1, StmtType::Print);
    shared_ptr<StmtInfo> s6 = MockUtilities::createStmtInfo(1, StmtType::Read);
    unordered_set<VarRef> varRefs = unordered_set<VarRef>({"x", "y", "z"});
    REQUIRE(Modifies::validate(&store, s1, varRefs));
    REQUIRE(Modifies::validate(&store, s2, varRefs));
    REQUIRE_FALSE(Modifies::validate(&store, s3, varRefs));
    REQUIRE(Modifies::validate(&store, s4, varRefs));
    REQUIRE_THROWS_AS(Modifies::validate(&store, s5, varRefs), std::exception);
    REQUIRE_FALSE(Modifies::validate(&store, s6, varRefs));
}
