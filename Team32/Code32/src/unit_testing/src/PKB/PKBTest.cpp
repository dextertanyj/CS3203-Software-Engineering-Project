#include "catch.hpp"

#include "PKB/PKB.h"

TEST_CASE("Follows Methods") {
    StmtRef s1 = 1;
    StmtRef s2 = 2;
    StmtRef s3 = 3;
    PKB pkb = PKB();

    SECTION("Check validity of arguments") {
        // Verify that normal setting works.
        CHECK_NOTHROW(pkb.setFollows(s1,s2));
        CHECK_NOTHROW(pkb.setFollows(s2,s3));
        // CHECK_NOTHROW(pkb.setFollows(1, INT32_MAX));

        // Verify that improper arguments lead to an exception thrown.
        CHECK_THROWS(pkb.setFollows(-1, 1));
        CHECK_THROWS(pkb.setFollows(2, 1));
        CHECK_THROWS(pkb.setFollows(3, 3));
        CHECK_THROWS(pkb.setFollows(0, 2));
        CHECK_THROWS(pkb.setFollows(3, -4));
        CHECK_THROWS(pkb.getFollower(-1));
        CHECK_THROWS(pkb.getFollowee(0));
    }

    SECTION("Basic set and get follows") {
        pkb.setFollows(s1, s2);
        pkb.setFollows(s2, s3);

        // Ensure simple follow relation is stored successfully.
        CHECK( pkb.checkFollows(s1, s2));
        CHECK( pkb.checkFollows(s2, s3));
        CHECK( pkb.getFollower(s1) == s2);
        CHECK( pkb.getFollowee(s3) == s2);

        // Ensure Follows* behavior does not appear for simple follow.
        CHECK_FALSE( pkb.checkFollows(s1, s3));

        CHECK_FALSE(pkb.checkFollows(s1, s1));
        CHECK_FALSE(pkb.checkFollows(s2, s1));
        CHECK_FALSE(pkb.checkFollows(s3, s2));
    }
}

TEST_CASE("Parents Methods") {
    StmtRef s1 = 1;
    StmtRef s2 = 2;
    StmtRef s3 = 3;
    PKB pkb = PKB();

    SECTION("Check validity of arguments") {
        // Verify that normal setting works.
        CHECK_NOTHROW(pkb.setParent(s1,s2));
        CHECK_NOTHROW(pkb.setParent(s2,s3));
        CHECK_NOTHROW(pkb.setParent(1, INT32_MAX));

        // Verify that improper arguments lead to an exception thrown.
        CHECK_THROWS(pkb.setParent(-1, 1));
        CHECK_THROWS(pkb.setParent(2, 1));
        CHECK_THROWS(pkb.setParent(3, 3));
        CHECK_THROWS(pkb.setParent(0, 2));
        CHECK_THROWS(pkb.setParent(3, -4));
        CHECK_THROWS(pkb.getParent(-1));
        CHECK_THROWS(pkb.getChildren(0));
    }

    SECTION("Basic set and get parents") {
        StmtRef s4 = 4;

        pkb.setParent(s1, s2);
        pkb.setParent(s2, s3);
        pkb.setParent(s2, s4);

        // Ensure simple parent relation is stored successfully.
        CHECK( pkb.checkParents(s1, s2));
        CHECK( pkb.checkParents(s2, s3));
        CHECK( pkb.getParent(s2) == s1);
        CHECK( pkb.getParent(s4) == s2);
        CHECK (pkb.getChildren(s1) == unordered_set<StmtRef>{ s2 });
        CHECK (pkb.getChildren(s2) == unordered_set<StmtRef>{ s3, s4 });

        // Ensure Parent* behavior does not appear for simple parent.
        CHECK_FALSE( pkb.checkParents(s1, s3));

        CHECK_FALSE(pkb.checkParents(s1, s1));
        CHECK_FALSE(pkb.checkParents(s2, s1));
        CHECK_FALSE(pkb.checkParents(s3, s2));
    }
}
