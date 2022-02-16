#include "MockUtilities.h"
#include "PKB/FollowsRelation.h"
#include "PKB/PKB.h"
#include "catch.hpp"

TEST_CASE("PKB::PKB Follows Methods Test") {
	PKB pkb = MockUtilities::generateFollowsTestPKB();
	StmtRef s_1 = 1;
	StmtRef s_2 = 2;
	StmtRef s_3 = 3;
	StmtRef s_4 = 4;
	StmtRef s_5 = 5;
	StmtRef s_6 = 6;
	StmtRef s_int_max = INT32_MAX;
	StmtRef s_minus_one = -1;

	SECTION("PKB::PKB::setFollows Test") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);
		// Invalid setting of negative statement number to statement map.
		CHECK_THROWS(pkb.setStmtType(-1, StmtType::Assign));

		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setFollows(s_1, s_2));
		CHECK_NOTHROW(pkb.setFollows(s_2, s_3));
		pkb = MockUtilities::generateFollowsTestPKB();

		// If s_1 already has a follower, it should not be able to have a new direct follower.
		CHECK_NOTHROW(pkb.setFollows(s_1, s_2));
		CHECK_THROWS(pkb.setFollows(s_1, s_int_max));
		pkb = MockUtilities::generateFollowsTestPKB();

		// If s_3 already follows s_2, then it should not be allowed to follow s_1.
		CHECK_NOTHROW(pkb.setFollows(s_2, s_3));
		CHECK_THROWS(pkb.setFollows(s_1, s_3));
		pkb = MockUtilities::generateFollowsTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setFollows(s_1, s_1));
		CHECK_THROWS(pkb.setFollows(s_minus_one, s_1));
		CHECK_THROWS(pkb.setFollows(s_2, s_1));
		CHECK_THROWS(pkb.setFollows(s_3, s_3));
		CHECK_THROWS(pkb.setFollows(s_minus_one, s_2));
		CHECK_THROWS(pkb.setFollows(s_3, s_minus_one));
	}

	SECTION("PKB::PKB::checkFollows Test") {
		pkb.setFollows(s_1, s_2);
		pkb.setFollows(s_2, s_3);

		// Ensure simple follow relation is stored successfully.
		CHECK(pkb.checkFollows(s_1, s_2));
		CHECK(pkb.checkFollows(s_2, s_3));

		// Ensure Follows* behavior does not appear for simple follow.
		CHECK_FALSE(pkb.checkFollows(s_1, s_3));

		// More negative cases.
		CHECK_FALSE(pkb.checkFollows(s_1, s_1));
		CHECK_FALSE(pkb.checkFollows(s_2, s_1));
		CHECK_FALSE(pkb.checkFollows(s_3, s_2));
	}

	SECTION("PKB::PKB::getFollower Test") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);

		pkb.setFollows(s_1, s_4);
		pkb.setFollows(s_2, s_3);
		pkb.setFollows(s_4, s_int_max);

		CHECK(pkb.getFollower(s_1)->reference == s_4);
		CHECK(pkb.getFollower(s_2)->reference == s_3);
		CHECK(pkb.getFollower(s_3) == nullptr);
		CHECK(pkb.getFollower(s_int_max) == nullptr);

		// Not stored into PKB.
		CHECK(pkb.getFollower(s_5) == nullptr);
		CHECK_THROWS_AS(pkb.getFollower(s_minus_one), std::invalid_argument);
	}

	SECTION("PKB::PKB::getPreceding Test") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);

		pkb.setFollows(s_1, s_4);
		pkb.setFollows(s_2, s_3);
		pkb.setFollows(s_4, s_int_max);

		CHECK(pkb.getPreceding(s_1) == nullptr);
		CHECK(pkb.getPreceding(s_2) == nullptr);
		CHECK(pkb.getPreceding(s_3)->reference == s_2);
		CHECK(pkb.getPreceding(s_4)->reference == s_1);
		CHECK(pkb.getPreceding(s_int_max)->reference == s_4);

		// Not stored in PKB.
		CHECK(pkb.getPreceding(s_5) == nullptr);
		CHECK_THROWS_AS(pkb.getPreceding(s_minus_one), std::invalid_argument);
	}

	// Nesting levels as such:
	// 1-->2----------->6
	//       3-->4-->5
	SECTION("PKB::PKB::getFollowerStar Test") {
		pkb.setFollows(s_1, s_2);
		pkb.setFollows(s_2, s_6);
		pkb.setFollows(s_3, s_4);
		pkb.setFollows(s_4, s_5);
		pkb.populateComplexRelations();
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_1 = {stmt_info_map.at(s_2), stmt_info_map.at(s_6)};
		CHECK(pkb.getFollowerStar(s_1) == expected_follow_star_set_s_1);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_2 = {stmt_info_map.at(s_6)};
		CHECK(pkb.getFollowerStar(s_2) == expected_follow_star_set_s_2);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_3 = {stmt_info_map.at(s_4), stmt_info_map.at(s_5)};
		CHECK(pkb.getFollowerStar(s_3) == expected_follow_star_set_s_3);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_4 = {stmt_info_map.at(s_5)};
		CHECK(pkb.getFollowerStar(s_4) == expected_follow_star_set_s_4);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_5 = {};
		CHECK(pkb.getFollowerStar(s_5) == expected_follow_star_set_s_5);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_int_max = {};
		CHECK(pkb.getFollowerStar(s_6) == expected_follow_star_set_s_int_max);
	}

	// Nesting levels as such:
	// 1-->2----------->6
	//       3-->4-->5
	SECTION("PKB::PKB::getPrecedingStar Test") {
		pkb.setFollows(s_1, s_2);
		pkb.setFollows(s_2, s_6);
		pkb.setFollows(s_3, s_4);
		pkb.setFollows(s_4, s_5);
		pkb.populateComplexRelations();
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_6 = {stmt_info_map.at(s_1), stmt_info_map.at(s_2)};
		CHECK(pkb.getPrecedingStar(s_6) == expected_preceding_star_set_s_6);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_5 = {stmt_info_map.at(s_3), stmt_info_map.at(s_4)};
		CHECK(pkb.getPrecedingStar(s_5) == expected_preceding_star_set_s_5);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_4 = {stmt_info_map.at(s_3)};
		CHECK(pkb.getPrecedingStar(s_4) == expected_preceding_star_set_s_4);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_3 = {};
		CHECK(pkb.getPrecedingStar(s_3) == expected_preceding_star_set_s_3);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_2 = {stmt_info_map.at(s_1)};
		CHECK(pkb.getPrecedingStar(s_2) == expected_preceding_star_set_s_2);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_1 = {};
		CHECK(pkb.getPrecedingStar(s_1) == expected_preceding_star_set_s_1);
	}
}

TEST_CASE("PKB::PKB Parent Methods Test") {
	StmtRef s_0 = 0;
	StmtRef s_1 = 1;
	StmtRef s_2 = 2;
	StmtRef s_3 = 3;
	StmtRef s_4 = 4;
	StmtRef s_5 = 5;
	StmtRef s_6 = 6;
	StmtRef s_7 = 7;
	StmtRef s_8 = 8;
	StmtRef s_9 = 9;
	StmtRef s_int_max = INT32_MAX;
	StmtRef s_minus_one = -1;
	PKB pkb = MockUtilities::generateParentTestPKB();

	SECTION("PKB::PKB::setParent Test") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);

		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setParent(s_1, s_2));
		CHECK_NOTHROW(pkb.setParent(s_2, s_3));

		// Parent statement can have multiple children
		CHECK_NOTHROW(pkb.setParent(s_1, s_int_max));

		// Same Parent-child relation cannot be set twice.
		CHECK_THROWS(pkb.setParent(s_1, s_2));
		CHECK_THROWS(pkb.setParent(s_2, s_3));

		// A child cannot have multiple parents.
		CHECK_THROWS(pkb.setParent(s_2, s_int_max));
		CHECK_THROWS(pkb.setParent(s_1, s_3));
		pkb = MockUtilities::generateParentTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setParent(s_minus_one, s_1));
		CHECK_THROWS(pkb.setParent(s_2, s_1));
		CHECK_THROWS(pkb.setParent(s_3, s_3));
		CHECK_THROWS(pkb.setParent(s_0, s_2));
		CHECK_THROWS(pkb.setParent(s_3, s_minus_one));
	}

	SECTION("PKB::PKB::checkParents Test") {
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();
		pkb.setParent(s_1, s_2);
		pkb.setParent(s_2, s_3);
		pkb.setParent(s_2, s_4);

		// Ensure simple parent relation is stored successfully.
		CHECK(pkb.checkParents(s_1, s_2));
		CHECK(pkb.checkParents(s_2, s_3));
		CHECK(pkb.checkParents(s_2, s_4));
		CHECK(pkb.getParent(s_2)->reference == s_1);
		CHECK(pkb.getParent(s_3)->reference == s_2);
		CHECK(pkb.getParent(s_4)->reference == s_2);

		shared_ptr<StmtInfo> expected_s_2_info = stmt_info_map.find(s_2)->second;
		shared_ptr<StmtInfo> expected_s_3_info = stmt_info_map.find(s_3)->second;
		shared_ptr<StmtInfo> expected_s_4_info = stmt_info_map.find(s_4)->second;

		// Check that s_1's direct child set only contains s_2.
		unordered_set<shared_ptr<StmtInfo>> expected_child_set_single = {expected_s_2_info};
		CHECK(pkb.getChildren(s_1) == expected_child_set_single);

		// Check that s_2's direct children set contains s_3 and s_4.
		unordered_set<shared_ptr<StmtInfo>> expected_child_set_s_2 = {expected_s_3_info, expected_s_4_info};
		CHECK(pkb.getChildren(s_2) == expected_child_set_s_2);

		// Ensure Parent* behavior does not appear for simple parent relation.
		CHECK_FALSE(pkb.checkParents(s_1, s_3));
		CHECK_FALSE(pkb.checkParents(s_1, s_4));

		// Test invalid check-parents cases.
		CHECK_FALSE(pkb.checkParents(s_1, s_1));
		CHECK_FALSE(pkb.checkParents(s_2, s_1));
		CHECK_FALSE(pkb.checkParents(s_3, s_2));
	}

	SECTION("PKB::PKB::getParent Test") {
		pkb.setParent(s_1, s_2);
		pkb.setParent(s_2, s_3);
		pkb.setParent(s_2, s_4);

		CHECK(pkb.getParent(s_1) == nullptr);  // Highest statement should have no parent.
		CHECK(pkb.getParent(s_2)->reference == 1);
		CHECK(pkb.getParent(s_3)->reference == 2);

		// Statement which was not stored in PKB.
		CHECK(pkb.getParent(s_5) == nullptr);
		// StmtRef <= 0 not allowed for get methods.
		CHECK_THROWS_AS(pkb.getParent(s_minus_one), std::invalid_argument);
	}

	// Nesting levels:
	// 1--|
	//  2------>5--->7--|
	//    3->4-|  6-|  8--|
	//                   9--|
	SECTION("PKB::PKB::getParentStar Test") {
		pkb.setParent(s_1, s_2);
		pkb.setParent(s_2, s_3);
		pkb.setParent(s_2, s_4);
		pkb.setParent(s_1, s_5);
		pkb.setParent(s_5, s_6);
		pkb.setParent(s_1, s_7);
		pkb.setParent(s_7, s_8);
		pkb.setParent(s_8, s_9);
		pkb.populateComplexRelations();

		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();
		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_1 = {};
		CHECK(pkb.getParentStar(s_1) == expected_parent_star_s_1);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_2 = {stmt_info_map.at(s_1)};
		CHECK(pkb.getParentStar(s_2) == expected_parent_star_s_2);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_3 = {stmt_info_map.at(s_1), stmt_info_map.at(s_2)};
		CHECK(pkb.getParentStar(s_3) == expected_parent_star_s_3);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_4 = {stmt_info_map.at(s_1), stmt_info_map.at(s_2)};
		CHECK(pkb.getParentStar(s_4) == expected_parent_star_s_4);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_5 = {stmt_info_map.at(s_1)};
		CHECK(pkb.getParentStar(s_5) == expected_parent_star_s_5);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_6 = {stmt_info_map.at(s_1), stmt_info_map.at(s_5)};
		CHECK(pkb.getParentStar(s_6) == expected_parent_star_s_6);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_7 = {stmt_info_map.at(s_1)};
		CHECK(pkb.getParentStar(s_7) == expected_parent_star_s_7);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_8 = {stmt_info_map.at(s_1), stmt_info_map.at(s_7)};
		CHECK(pkb.getParentStar(s_8) == expected_parent_star_s_8);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s_9 = {stmt_info_map.at(s_1), stmt_info_map.at(s_7),
		                                                                stmt_info_map.at(s_8)};
		CHECK(pkb.getParentStar(s_9) == expected_parent_star_s_9);
	}

	// Nesting levels:
	// 1--|
	//  2------>5--->7--|
	//    3->4-|  6-|  8--|
	//                   9--|
	SECTION("PKB::PKB::getChildStar Test") {
		pkb.setParent(s_1, s_2);
		pkb.setParent(s_2, s_3);
		pkb.setParent(s_2, s_4);
		pkb.setParent(s_1, s_5);
		pkb.setParent(s_5, s_6);
		pkb.setParent(s_1, s_7);
		pkb.setParent(s_7, s_8);
		pkb.setParent(s_8, s_9);
		pkb.populateComplexRelations();
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_1 = {
			stmt_info_map.at(s_2), stmt_info_map.at(s_3), stmt_info_map.at(s_4), stmt_info_map.at(s_5),
			stmt_info_map.at(s_6), stmt_info_map.at(s_7), stmt_info_map.at(s_8), stmt_info_map.at(s_9),
		};
		CHECK(pkb.getChildStar(s_1) == expected_child_star_s_1);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_2 = {stmt_info_map.at(s_3), stmt_info_map.at(s_4)};
		CHECK(pkb.getChildStar(s_2) == expected_child_star_s_2);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_3 = {};
		CHECK(pkb.getChildStar(s_3) == expected_child_star_s_3);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_4 = {};
		CHECK(pkb.getChildStar(s_4) == expected_child_star_s_4);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_5 = {stmt_info_map.at(s_6)};
		CHECK(pkb.getChildStar(s_5) == expected_child_star_s_5);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_6 = {};
		CHECK(pkb.getChildStar(s_6) == expected_child_star_s_6);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_7 = {stmt_info_map.at(s_8), stmt_info_map.at(s_9)};
		CHECK(pkb.getChildStar(s_7) == expected_child_star_s_7);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_8 = {stmt_info_map.at(s_9)};
		CHECK(pkb.getChildStar(s_8) == expected_child_star_s_8);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s_9 = {};
		CHECK(pkb.getChildStar(s_9) == expected_child_star_s_9);
	}
}
