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
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Read);
	return pkb;
}

TEST_CASE("Follows Methods") {
	PKB pkb = generateFollowTestPKB();
	pkb.setStmtType(INT32_MAX, StmtType::Read);
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

		// If s3 already follows s2, then it should not be allowed to follow s1.
		CHECK_NOTHROW(pkb.setFollows(s2, s3));
		CHECK_THROWS(pkb.setFollows(s1, s3));
		pkb = generateFollowTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setFollows(s1, s1));
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

TEST_CASE("FollowStar Methods") {
	StmtRef s1 = 1;
	StmtRef s2 = 2;
	StmtRef s3 = 3;
	StmtRef s4 = 4;
	StmtRef s5 = 5;
	StmtRef s6 = 6;

	PKB pkb = generateFollowTestPKB();
	// Nesting levels as such:
	// 1-->2----------->6
	//       3-->4-->5
	pkb.setFollows(s1, s2);
	pkb.setFollows(s2, s6);
	pkb.setFollows(s3, s4);
	pkb.setFollows(s4, s5);

	pkb.populateComplexRelations();
	unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

	SECTION("Check Populate FollowStar Method") {
		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s1 = {
			stmt_info_map.at(s2),
			stmt_info_map.at(s6)
		};
		CHECK(pkb.getFollowerStar(s1) == expected_follow_star_set_s1);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s2 = {stmt_info_map.at(s6)};
		CHECK(pkb.getFollowerStar(s2) == expected_follow_star_set_s2);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s3 = {
			stmt_info_map.at(s4),
			stmt_info_map.at(s5)
		};
		CHECK(pkb.getFollowerStar(s3) == expected_follow_star_set_s3);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s4 = {stmt_info_map.at(s5)};
		CHECK(pkb.getFollowerStar(s4) == expected_follow_star_set_s4);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s5 = {};
		CHECK(pkb.getFollowerStar(s5) == expected_follow_star_set_s5);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_int_max = {};
		CHECK(pkb.getFollowerStar(s6) == expected_follow_star_set_s_int_max);
	}

	SECTION("Check Populate PrecedingStar Method") {
		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s6 = {
			stmt_info_map.at(s1),
			stmt_info_map.at(s2)
		};
		CHECK(pkb.getPrecedingStar(s6) == expected_preceding_star_set_s6);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s5 = {
			stmt_info_map.at(s3),
			stmt_info_map.at(s4)
		};
		CHECK(pkb.getPrecedingStar(s5) == expected_preceding_star_set_s5);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s4 = {stmt_info_map.at(s3)};
		CHECK(pkb.getPrecedingStar(s4) == expected_preceding_star_set_s4);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s3 = {};
		CHECK(pkb.getPrecedingStar(s3) == expected_preceding_star_set_s3);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s2 = {stmt_info_map.at(s1)};
		CHECK(pkb.getPrecedingStar(s2) == expected_preceding_star_set_s2);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s1 = {};
		CHECK(pkb.getPrecedingStar(s1) == expected_preceding_star_set_s1);
	}
}
