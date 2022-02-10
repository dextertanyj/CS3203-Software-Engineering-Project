#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "memory"

/*
 * Tests Follow Relation Methods in PKB.
 */

TEST_CASE("Follows Methods") {
	PKB pkb = PKB();
	StmtRef s1 = 1;
	StmtRef s2 = 2;
	StmtRef s3 = 3;
	StmtRef sIntMax = INT32_MAX;
	StmtRef sMinusOne = -1;
	CHECK_NOTHROW(pkb.setStmtType(1, StmtType::Assign));
	CHECK_NOTHROW(pkb.setStmtType(2, StmtType::IfStmt));
	CHECK_NOTHROW(pkb.setStmtType(3, StmtType::Print));
	CHECK_NOTHROW(pkb.setStmtType(INT32_MAX, StmtType::Read));
	CHECK_THROWS(pkb.setStmtType(-1, StmtType::Assign));

	SECTION("Check validity of arguments") {
		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setFollows(s1, s2));
		CHECK_NOTHROW(pkb.setFollows(s2, s3));
		pkb.clear();

		// If s1 already has a follower, it should not be able to have a new direct follower.
		CHECK_NOTHROW(pkb.setFollows(s1, s2));
		CHECK_THROWS(pkb.setFollows(s1, sIntMax));
		pkb.clear();

		// If s3 already follows s2, then it should be allowed to follow s1.
		CHECK_NOTHROW(pkb.setFollows(s2, s3));
		CHECK_THROWS(pkb.setFollows(s1, s3));
		pkb.clear();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setFollows(sMinusOne, s1));
		CHECK_THROWS(pkb.setFollows(s2, s1));
		CHECK_THROWS(pkb.setFollows(s3, s3));
		CHECK_THROWS(pkb.setFollows(sMinusOne, s2));
		CHECK_THROWS(pkb.setFollows(s3, sMinusOne));
		CHECK_THROWS(pkb.getFollower(sMinusOne));
		CHECK_THROWS(pkb.getPreceding(s1));
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

		CHECK_FALSE(pkb.checkFollows(s1, s1));
		CHECK_FALSE(pkb.checkFollows(s2, s1));
		CHECK_FALSE(pkb.checkFollows(s3, s2));
		pkb.clear();
	}
}

// TODO: Test Follow* functionality.