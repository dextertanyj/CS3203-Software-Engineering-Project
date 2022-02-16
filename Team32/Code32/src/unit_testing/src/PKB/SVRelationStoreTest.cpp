#include "PKB/SVRelationStore.h"

#include "MockUtilities.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::SVRelationStore") {
    SVRelationStore uses_store = SVRelationStore<Uses>();
    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(5, StmtType::Print);
    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(6, StmtType::Assign);
    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(7, StmtType::IfStmt);
    shared_ptr<StmtInfo> s4 = MockUtilities::createStmtInfo(1, StmtType::Read);

    VarRefSet v1 = {"x", "y", "z"};
    VarRefSet v2 = {"a", "b"};
    VarRefSet v3 = {"a", ""};
    VarRefSet v4 = {};
    VarRefSet v5 = {"a"};

    SECTION("PKB::SVRelationStore::set by VarRef Test") {
        // Variable length less than 1
        REQUIRE_THROWS_AS(uses_store.set(s1, ""), invalid_argument);
        // Read statement does not use a variable
        REQUIRE_THROWS_AS(uses_store.set(s4, "x"), invalid_argument);
        uses_store.set(s1, "x");
        uses_store.set(s2, "y");
        uses_store.set(s2, "a");
        uses_store.set(s3, "x");

        // Print statement cannot use more than one variable
        CHECK_THROWS(uses_store.set(s1, "a"));
        CHECK(uses_store.check(5, "x"));
        CHECK(uses_store.check(6, "y"));
        CHECK(uses_store.check(6, "a"));
        CHECK(uses_store.check(7, "x"));
    }

    SECTION("PKB::SVRelationStore::set by VarRefSet Test") {
        // Variable set contains empty string
        REQUIRE_THROWS_AS(uses_store.set(s2, v3), invalid_argument);
        // Read statement does not use variables
        REQUIRE_THROWS_AS(uses_store.set(s4, v2), invalid_argument);
        uses_store.set(s1, v5);
        uses_store.set(s2, v1);
        uses_store.set(s3, v4);
        // Print statement cannot use more than one variable
        CHECK_THROWS(uses_store.set(s1, v1));
        CHECK(uses_store.check(5, "a"));
        CHECK(uses_store.check(6, "x"));
        CHECK(uses_store.check(6, "y"));
        CHECK(uses_store.check(6, "z"));
    }

    SECTION("PKB::SVRelationStore::check Test") {
        uses_store.set(s1, "x");
        uses_store.set(s2, v1);
        uses_store.set(s3, v2);

        // StmtRef index less than or equal to 0
        REQUIRE_THROWS_AS(uses_store.check(-1, "x"), invalid_argument);
        // Variable as an empty string
        REQUIRE_THROWS_AS(uses_store.check(2, ""), invalid_argument);
        // StmtRef does not exist in map
        CHECK_FALSE(uses_store.check(2, "x"));
        CHECK_FALSE(uses_store.check(5, "y"));
        CHECK(uses_store.check(5, "x"));
        CHECK(uses_store.check(6, "x"));
        CHECK(uses_store.check(6, "y"));
        CHECK(uses_store.check(6, "z"));
        CHECK(uses_store.check(7, "a"));
        CHECK(uses_store.check(7, "b"));
    }

    SECTION("PKB::SVRelationStore::getByStmt Test") {
        uses_store.set(s1, "a");
        uses_store.set(s2, v1);
        uses_store.set(s3, v2);

        // StmtRef index less than or equal to 0
        REQUIRE_THROWS_AS(uses_store.getByStmt(0), invalid_argument);
        CHECK(uses_store.getByStmt(5) == v5);
        CHECK(uses_store.getByStmt(6) == v1);
        CHECK(uses_store.getByStmt(7) == v2);
        CHECK(uses_store.getByStmt(2).empty());
    }

    SECTION("PKB::SVRelationStore::getByVar Test") {
        uses_store.set(s1, "a");
        uses_store.set(s2, v1);
        uses_store.set(s3, v2);

        // Variable as an empty string
        REQUIRE_THROWS_AS(uses_store.getByVar(""), invalid_argument);
        unordered_set<shared_ptr<StmtInfo>> expected_set_a = { s1, s3 };
        unordered_set<shared_ptr<StmtInfo>> expected_set_x = { s2 };
        unordered_set<shared_ptr<StmtInfo>> expected_set_y = { s2 };
        unordered_set<shared_ptr<StmtInfo>> expected_set_b = { s3 };
        CHECK(uses_store.getByVar("a") == expected_set_a);
        CHECK(uses_store.getByVar("x") == expected_set_x);
        CHECK(uses_store.getByVar("y") == expected_set_y);
        CHECK(uses_store.getByVar("b") == expected_set_b);
    }
}
