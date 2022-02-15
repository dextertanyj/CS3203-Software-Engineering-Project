#include "PKB/Uses.h"

#include "MockUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::Uses::validate One Var Test") {
    SVRelationStore<Uses> store = SVRelationStore<Uses>();
    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(1, StmtType::IfStmt);
    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(1, StmtType::Assign);
    shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(1, StmtType::Call);
    shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(1, StmtType::Print);
    shared_ptr<StmtInfo> s6 = MockUtilities::createStmtInfo(1, StmtType::Read);
    REQUIRE(Uses::validate(&store, s1, "x"));
    REQUIRE(Uses::validate(&store, s2, "x"));
    REQUIRE(Uses::validate(&store, s3, "x"));
    REQUIRE(Uses::validate(&store, s4, "x"));
    REQUIRE(Uses::validate(&store, s5, "x"));
    REQUIRE_THROWS_AS(Uses::validate(&store, s6, "x"), std::exception);
    store.set(s1, "x");
    store.set(s2, "x");
    store.set(s3, "x");
    store.set(s4, "x");
    store.set(s5, "x");
    REQUIRE(Uses::validate(&store, s1, "y"));
    REQUIRE(Uses::validate(&store, s2, "y"));
    REQUIRE(Uses::validate(&store, s3, "y"));
    REQUIRE(Uses::validate(&store, s4, "y"));
    REQUIRE_FALSE(Uses::validate(&store, s5, "y"));
}

TEST_CASE("PKB::Uses::validate Multiple Var Test") {
    SVRelationStore<Uses> store = SVRelationStore<Uses>();
    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(1, StmtType::IfStmt);
    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(1, StmtType::Assign);
    shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(1, StmtType::Call);
    shared_ptr<StmtInfo> s5 = MockUtilities::createStmtInfo(1, StmtType::Print);
    shared_ptr<StmtInfo> s6 = MockUtilities::createStmtInfo(1, StmtType::Read);
    unordered_set<VarRef> varRefs = unordered_set<VarRef>({"x", "y", "z"});
    REQUIRE(Uses::validate(&store, s1, varRefs));
    REQUIRE(Uses::validate(&store, s2, varRefs));
    REQUIRE(Uses::validate(&store, s3, varRefs));
    REQUIRE(Uses::validate(&store, s4, varRefs));
    REQUIRE_FALSE(Uses::validate(&store, s5, varRefs));
    REQUIRE_THROWS_AS(Uses::validate(&store, s6, varRefs), std::exception);
}
