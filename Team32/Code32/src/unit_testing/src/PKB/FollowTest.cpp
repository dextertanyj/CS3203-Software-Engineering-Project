#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "memory"

/*
 * Tests Follow Relation Methods in PKB.
 */

PKB generateFollowTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
    pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(INT32_MAX, StmtType::Read);
	return pkb;
}

TEST_CASE("FollowStar Methods") {
    PKB pkb = generateFollowTestPKB();
    StmtRef s1 = 1;
    StmtRef s2 = 2;
    StmtRef s3 = 3;
    StmtRef s4 = 4;

    pkb.setFollows(s1, s2);
    pkb.setFollows(s2, s4);
    pkb.populateComplexRelations();

    unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

    SECTION("Check Populate FollowStar Method") {
        unordered_set<shared_ptr<StmtInfo>> expectedFollowStar = {stmt_info_map.at(s2),
                                                                  stmt_info_map.at(s4)};
        CHECK(pkb.getFollowerStar(s1) == expectedFollowStar);
    }

    SECTION("Check Populate PrecedingStar Method") {
        unordered_set<shared_ptr<StmtInfo>> expectedPrecedingStar = {stmt_info_map.at(s1),
                                                                  stmt_info_map.at(s2)};
        CHECK(pkb.getPrecedingStar(s4) == expectedPrecedingStar);
    }

}

TEST_CASE("Follows Methods") {
	PKB pkb = generateFollowTestPKB();
	StmtRef s1 = 1;
	StmtRef s2 = 2;
	StmtRef s3 = 3;
	StmtRef sIntMax = INT32_MAX;
	StmtRef sMinusOne = -1;

	SECTION("Check validity of arguments") {
		// Invalid setting of negative statement number to statement map.
		CHECK_THROWS(pkb.setStmtType(-1, StmtType::Assign));

		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setFollows(s1, s2));
		CHECK_NOTHROW(pkb.setFollows(s2, s3));
		pkb = generateFollowTestPKB();

		// If s1 already has a follower, it should not be able to have a new direct follower.
		CHECK_NOTHROW(pkb.setFollows(s1, s2));
		CHECK_THROWS(pkb.setFollows(s1, sIntMax));
		pkb = generateFollowTestPKB();

		// If s3 already follows s2, then it should be allowed to follow s1.
		CHECK_NOTHROW(pkb.setFollows(s2, s3));
		CHECK_THROWS(pkb.setFollows(s1, s3));
		pkb = generateFollowTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setFollows(sMinusOne, s1));
		CHECK_THROWS(pkb.setFollows(s2, s1));
		CHECK_THROWS(pkb.setFollows(s3, s3));
		CHECK_THROWS(pkb.setFollows(sMinusOne, s2));
		CHECK_THROWS(pkb.setFollows(s3, sMinusOne));
		CHECK_THROWS(pkb.getFollower(sMinusOne));
		CHECK(pkb.getPreceding(s1) == nullptr);
	}

	SECTION("Check setting and getting of follower/preceding statement") {
		pkb.setFollows(s1, s2);
		pkb.setFollows(s2, s3);

		// Ensure simple follow relation is stored successfully.
		CHECK(pkb.checkFollows(s1, s2));
		CHECK(pkb.checkFollows(s2, s3));
		CHECK(pkb.getFollower(s1)->reference == s2);
		CHECK(pkb.getPreceding(s3)->reference == s2);

		// Ensure Follows* behavior does not appear for simple follow.
		CHECK_FALSE(pkb.checkFollows(s1, s3));

		// More negative cases.
		CHECK_FALSE(pkb.checkFollows(s1, s1));
		CHECK_FALSE(pkb.checkFollows(s2, s1));
		CHECK_FALSE(pkb.checkFollows(s3, s2));
	}
}

// TODO: Test Follow* functionality.