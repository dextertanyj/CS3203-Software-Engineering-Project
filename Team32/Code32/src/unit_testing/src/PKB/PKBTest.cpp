#include "MockUtilities.h"
#include "PKB/FollowsRelation.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include <climits>

TEST_CASE("PKB::PKB Follows Methods Test") {
	PKB pkb = MockUtilities::generateFollowsTestPKB();
	StmtRef s_1 = 1;
	StmtRef s_2 = 2;
	StmtRef s_3 = 3;
	StmtRef s_4 = 4;
	StmtRef s_5 = 5;
	StmtRef s_6 = 6;
	StmtRef s_max = SIZE_T_MAX;
	StmtRef s_zero = 0;

	SECTION("PKB::PKB::setFollows Test") {
		pkb.setStmtType(SIZE_T_MAX, StmtType::Read);
		// Invalid setting of negative statement number to statement map.
		CHECK_THROWS(pkb.setStmtType(0, StmtType::Assign));

		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setFollows(s_1, s_2));
		CHECK_NOTHROW(pkb.setFollows(s_2, s_3));
		pkb = MockUtilities::generateFollowsTestPKB();

		// If s_1 already has a follower, it should not be able to have a new direct follower.
		CHECK_NOTHROW(pkb.setFollows(s_1, s_2));
		CHECK_THROWS(pkb.setFollows(s_1, s_max));
		pkb = MockUtilities::generateFollowsTestPKB();

		// If s_3 already follows s_2, then it should not be allowed to follow s_1.
		CHECK_NOTHROW(pkb.setFollows(s_2, s_3));
		CHECK_THROWS(pkb.setFollows(s_1, s_3));
		pkb = MockUtilities::generateFollowsTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setFollows(s_1, s_1));
		CHECK_THROWS(pkb.setFollows(s_zero, s_1));
		CHECK_THROWS(pkb.setFollows(s_2, s_1));
		CHECK_THROWS(pkb.setFollows(s_3, s_3));
		CHECK_THROWS(pkb.setFollows(s_zero, s_2));
		CHECK_THROWS(pkb.setFollows(s_3, s_zero));
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
		pkb.setStmtType(SIZE_T_MAX, StmtType::Read);

		pkb.setFollows(s_1, s_4);
		pkb.setFollows(s_2, s_3);
		pkb.setFollows(s_4, s_max);

		CHECK(pkb.getFollower(s_1)->reference == s_4);
		CHECK(pkb.getFollower(s_2)->reference == s_3);
		CHECK(pkb.getFollower(s_3) == nullptr);
		CHECK(pkb.getFollower(s_max) == nullptr);

		// Not stored into PKB.
		CHECK(pkb.getFollower(s_5) == nullptr);
		CHECK_THROWS_AS(pkb.getFollower(s_zero), std::invalid_argument);
	}

	SECTION("PKB::PKB::getPreceding Test") {
		pkb.setStmtType(SIZE_T_MAX, StmtType::Read);

		pkb.setFollows(s_1, s_4);
		pkb.setFollows(s_2, s_3);
		pkb.setFollows(s_4, s_max);

		CHECK(pkb.getPreceding(s_1) == nullptr);
		CHECK(pkb.getPreceding(s_2) == nullptr);
		CHECK(pkb.getPreceding(s_3)->reference == s_2);
		CHECK(pkb.getPreceding(s_4)->reference == s_1);
		CHECK(pkb.getPreceding(s_max)->reference == s_4);

		// Not stored in PKB.
		CHECK(pkb.getPreceding(s_5) == nullptr);
		CHECK_THROWS_AS(pkb.getPreceding(s_zero), std::invalid_argument);
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

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_max = {};
		CHECK(pkb.getFollowerStar(s_6) == expected_follow_star_set_s_max);
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
	StmtRef s_max = SIZE_T_MAX;
	StmtRef s_zero = 0;
	PKB pkb = MockUtilities::generateParentTestPKB();

	SECTION("PKB::PKB::setParent Test") {
		pkb.setStmtType(SIZE_T_MAX, StmtType::Read);

		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setParent(s_1, s_2));
		CHECK_NOTHROW(pkb.setParent(s_2, s_3));

		// Parent statement can have multiple children
		CHECK_NOTHROW(pkb.setParent(s_1, s_max));

		// Same Parent-child relation cannot be set twice.
		CHECK_THROWS(pkb.setParent(s_1, s_2));
		CHECK_THROWS(pkb.setParent(s_2, s_3));

		// A child cannot have multiple parents.
		CHECK_THROWS(pkb.setParent(s_2, s_max));
		CHECK_THROWS(pkb.setParent(s_1, s_3));
		pkb = MockUtilities::generateParentTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setParent(s_zero, s_1));
		CHECK_THROWS(pkb.setParent(s_2, s_1));
		CHECK_THROWS(pkb.setParent(s_3, s_3));
		CHECK_THROWS(pkb.setParent(s_0, s_2));
		CHECK_THROWS(pkb.setParent(s_3, s_zero));
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
		CHECK_THROWS_AS(pkb.getParent(s_zero), std::invalid_argument);
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

TEST_CASE("PKB::Uses Methods Test") {
    PKB pkb = MockUtilities::generateUsesTestPKB();
    unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();
    StmtRef s1 = 1;
    StmtRef s2 = 2;
    StmtRef s3 = 3;
    StmtRef s4 = 4;
    StmtRef s_max = SIZE_T_MAX;
    StmtRef s_zero = 0;
    VarRef x = "x";
    VarRef y = "y";
    VarRef z = "z";
    VarRef a = "a";
    VarRef xyz = "xyz";
    VarRefSet v1 = { x, y, z };
    VarRefSet v2 = { "a" };
    VarRefSet v3 = { x, "" };
    shared_ptr<StmtInfo> p1 = stmt_info_map.at(s1);
    shared_ptr<StmtInfo> p2 = stmt_info_map.at(s2);
    shared_ptr<StmtInfo> p3 = stmt_info_map.at(s3);

    SECTION("PKB::setUses by Var Test") {
        // Invalid arguments should throw an error
        REQUIRE_THROWS_AS(pkb.setUses(s_zero, x), invalid_argument);
        REQUIRE_THROWS_AS(pkb.setUses(s1, ""), invalid_argument);

        // StmtRef does not exist in Statement Store
        CHECK_THROWS(pkb.setUses(s4, x));

        // 1 statement, multiple vars.
        CHECK_NOTHROW(pkb.setUses(s1, x));
        CHECK_NOTHROW(pkb.setUses(s1, y));
        CHECK_NOTHROW(pkb.setUses(s1, z));

        // 1 var, multiple statements.
        CHECK_NOTHROW(pkb.setUses(s2, x));
        CHECK_NOTHROW(pkb.setUses(s3, x));
    }

    SECTION("PKB::setUses by VarSet Test") {
        REQUIRE_THROWS_AS(pkb.setUses(s_zero, v2), invalid_argument);
        REQUIRE_THROWS_AS(pkb.setUses(s1, v3), invalid_argument);

        // StmtRef does not exist in Statement Store
        CHECK_THROWS(pkb.setUses(s4, v1));

        // 1 statement, multiple vars.
        CHECK_NOTHROW(pkb.setUses(s1, v1));
        CHECK_NOTHROW(pkb.setUses(s1, v2));
        CHECK_NOTHROW(pkb.setUses(s2, v2));

        // Multiple vars for print statement
        CHECK_THROWS(pkb.setUses(s3, v1));
    }

    SECTION("PKB::checkUses Test") {
        pkb.setUses(s1, v1);
        pkb.setUses(s2, x);
        pkb.setUses(s3, x);

        CHECK(pkb.checkUses(s1,x));
        CHECK(pkb.checkUses(s1,y));
        CHECK(pkb.checkUses(s1,z));
        CHECK(pkb.checkUses(s2,x));
        CHECK(pkb.checkUses(s3,x));

        // Negative Cases
        CHECK_FALSE(pkb.checkUses(s_max, x));
        CHECK_FALSE(pkb.checkUses(s2, y));
        CHECK_FALSE(pkb.checkUses(s3, z));

        // Invalid arguments
        CHECK_THROWS(pkb.checkUses(s_zero, x));
        CHECK_THROWS(pkb.checkUses(s_max, ""));
    }

    SECTION("PKB::getUsesByVar Test") {
        pkb.setUses(s1, v1);
        pkb.setUses(s2, x);
        pkb.setUses(s3, x);

        unordered_set<shared_ptr<StmtInfo>> expected_set_1 = {p1, p2, p3};
        unordered_set<shared_ptr<StmtInfo>> expected_set_2 = {p1};
        unordered_set<shared_ptr<StmtInfo>> expected_set_3 = {p1};

        CHECK(pkb.getUsesByVar(x) == expected_set_1);
        CHECK(pkb.getUsesByVar(y) == expected_set_2);
        CHECK(pkb.getUsesByVar(z) == expected_set_3);

        CHECK(pkb.getUsesByVar(xyz).empty());
        CHECK_THROWS(pkb.getUsesByVar(""));
    }

    SECTION("PKB::getUsesByStmt Test") {
        pkb.setUses(s1, v1);
        pkb.setUses(s2, y);
        pkb.setUses(s2, z);
        pkb.setUses(s3, x);
        pkb.setUses(s_max, xyz);

        unordered_set<VarRef> used_by_s1 = {x, y, z};
        unordered_set<VarRef> used_by_s2 = {y, z};
        unordered_set<VarRef> used_by_s3 = {x};
        unordered_set<VarRef> used_by_s_max = {xyz};

        CHECK(pkb.getUsesByStmt(s1) == used_by_s1);
        CHECK(pkb.getUsesByStmt(s2) == used_by_s2);
        CHECK(pkb.getUsesByStmt(s3) == used_by_s3);
        CHECK(pkb.getUsesByStmt(s_max) == used_by_s_max);

        // Invalid arguments
        CHECK_THROWS(pkb.getUsesByStmt(s_zero));
        CHECK_THROWS(pkb.getUsesByStmt(0));
    }
}

TEST_CASE("PKB::Modifies Methods Test") {
    PKB pkb = MockUtilities::generateModifyTestPKB();
    unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();
    StmtRef s1 = 1;
    StmtRef s2 = 2;
    StmtRef s3 = 3;
    StmtRef s4 = 4;
    StmtRef s_max = SIZE_T_MAX;
    StmtRef s_zero = 0;
    VarRef x = "x";
    VarRef y = "y";
    VarRef z = "z";
    VarRef xyz = "xyz";
    VarRefSet v1 = { x, y, z };
    VarRefSet v2 = { "x" };
    VarRefSet v3 = { x, "" };
    shared_ptr<StmtInfo> p1 = stmt_info_map.at(s1);
    shared_ptr<StmtInfo> p2 = stmt_info_map.at(s2);
    shared_ptr<StmtInfo> p3 = stmt_info_map.at(s3);

    SECTION("PKB::setModifies by Var Test") {
        // StmtRef does not exist in Statement Store
        CHECK_THROWS(pkb.setModifies(s4, v1));

        // Throw error for invalid arguments
        CHECK_THROWS(pkb.setModifies(s_zero, x));
        CHECK_THROWS(pkb.setModifies(s1, ""));
        CHECK_THROWS(pkb.setModifies(s_zero, ""));

        CHECK_NOTHROW(pkb.setModifies(s1, x));
        // Same variable can be modified by different statements
        CHECK_NOTHROW(pkb.setModifies(s2, x));
        CHECK_NOTHROW(pkb.setModifies(s3, z));

        // Same statement cannot modify more than one variable
        CHECK_THROWS(pkb.setModifies(s1, y));
    }

    SECTION("PKB::setModifies by VarSet Test") {
        REQUIRE_THROWS_AS(pkb.setModifies(s_zero, v2), invalid_argument);
        REQUIRE_THROWS_AS(pkb.setModifies(s1, v3), invalid_argument);

        // StmtRef does not exist in Statement Store
        CHECK_THROWS(pkb.setModifies(s4, v1));

        // More than one variable in varset
        CHECK_THROWS(pkb.setModifies(s1, v1));

        CHECK_NOTHROW(pkb.setModifies(s1, v2));
        CHECK_NOTHROW(pkb.setModifies(s2, v2));
    }

    SECTION("PKB::checkModifies Test") {
        pkb.setModifies(s1, v2);
        pkb.setModifies(s2, y);
        pkb.setModifies(s3, z);
        pkb.setModifies(s_max, x);

        CHECK(pkb.checkModifies(s1, x));
        CHECK(pkb.checkModifies(s2, y));
        CHECK(pkb.checkModifies(s3, z));
        CHECK(pkb.checkModifies(s_max, x));

        // Negative Cases
        CHECK_FALSE(pkb.checkModifies(s_max, z));
        CHECK_FALSE(pkb.checkModifies(s2, x));
        CHECK_FALSE(pkb.checkModifies(s3, y));

        // Invalid arguments
        CHECK_THROWS(pkb.checkModifies(s_zero, x));
        CHECK_THROWS(pkb.checkModifies(s_max, ""));
    }

    SECTION("PKB::getModifiesByVar Test") {
        pkb.setModifies(s1, v2);
        pkb.setModifies(s2, x);
        pkb.setModifies(s3, y);

        unordered_set<shared_ptr<StmtInfo>> expected_set_1 = {p1, p2};
        unordered_set<shared_ptr<StmtInfo>> expected_set_2 = {p3};

        CHECK(pkb.getModifiesByVar(x) == expected_set_1);
        CHECK(pkb.getModifiesByVar(y) == expected_set_2);
        CHECK(pkb.getModifiesByVar(xyz).empty());
    }

    SECTION("PKB::getModifiesByStmt Test") {
        pkb.setModifies(s1, v2);
        pkb.setModifies(s2, x);
        pkb.setModifies(s3, y);

        CHECK(pkb.getModifiesByStmt(s1) == unordered_set<VarRef>{x});
        CHECK(pkb.getModifiesByStmt(s2) == unordered_set<VarRef>{x});
        CHECK(pkb.getModifiesByStmt(s3) == unordered_set<VarRef>{y});

        CHECK(pkb.getModifiesByStmt(s4).empty());
    }




}
