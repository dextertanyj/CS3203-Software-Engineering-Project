#include "Common/TypeDefs.h"
#include "PKB/FollowStore.cpp"
#include "catch.hpp"
#include "memory"

TEST_CASE("Follows Methods") {
	StmtInfo stmt1 = {1, StmtType::Assign};
	StmtInfo stmt2 = {2, StmtType::IfStmt};
	StmtInfo stmt3 = {3, StmtType::Print};
	StmtInfo stmtIntMax = {INT32_MAX, StmtType::Read};
	StmtInfo stmtNegative = {-1, StmtType::Assign};
	auto stmt1Ptr = make_shared<StmtInfo>(stmt1);
	auto stmt2Ptr = make_shared<StmtInfo>(stmt2);
	auto stmt3Ptr = make_shared<StmtInfo>(stmt3);
	auto stmtInfinityPtr = make_shared<StmtInfo>(stmtIntMax);
	auto stmtNegativePtr = make_shared<StmtInfo>(stmtNegative);
	FollowStore fs = FollowStore();

	SECTION("Check validity of arguments") {
		// Verify that normal setting works.
		CHECK_NOTHROW(fs.setFollows(stmt1Ptr, stmt2Ptr));
		CHECK_NOTHROW(fs.setFollows(stmt2Ptr, stmt3Ptr));
		CHECK_NOTHROW(fs.setFollows(stmt1Ptr, stmtInfinityPtr));
		fs.clear();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(fs.setFollows(stmtNegativePtr, stmt1Ptr));
		CHECK_THROWS(fs.setFollows(stmt2Ptr, stmt1Ptr));
		CHECK_THROWS(fs.setFollows(stmt3Ptr, stmt3Ptr));
		CHECK_THROWS(fs.setFollows(stmtNegativePtr, stmt2Ptr));
		CHECK_THROWS(fs.setFollows(stmt3Ptr, stmtNegativePtr));
		CHECK_THROWS(fs.getFollower(stmtNegativePtr));
		CHECK_THROWS(fs.getPreceding(stmt1Ptr));
	}

	SECTION("Check setting and getting of follower/preceding statement") {
		fs.setFollows(stmt1Ptr, stmt2Ptr);
		fs.setFollows(stmt2Ptr, stmt3Ptr);

		// Ensure simple follow relation is stored successfully.
		CHECK(fs.checkFollows(stmt1Ptr, stmt2Ptr));
		CHECK(fs.checkFollows(stmt2Ptr, stmt3Ptr));
		CHECK(fs.getFollower(stmt1Ptr) == stmt2Ptr);
		CHECK(fs.getPreceding(stmt3Ptr) == stmt2Ptr);

		// Ensure Follows* behavior does not appear for simple follow.
		CHECK_FALSE(fs.checkFollows(stmt1Ptr, stmt3Ptr));

		CHECK_FALSE(fs.checkFollows(stmt1Ptr, stmt1Ptr));
		CHECK_FALSE(fs.checkFollows(stmt2Ptr, stmt1Ptr));
		CHECK_FALSE(fs.checkFollows(stmt3Ptr, stmt2Ptr));
		fs.clear();
	}
}

// TODO: Test Follow* functionality.