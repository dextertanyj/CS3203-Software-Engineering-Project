#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "memory"

StmtRef s1 = 1;
StmtRef s2 = 2;
StmtRef s3 = 3;
StmtRef s4 = 4;
StmtRef s5 = 5;
StmtRef s6 = 6;
StmtRef s7 = 7;
StmtRef s8 = 8;
StmtRef s9 = 9;
StmtRef sIntMax = INT32_MAX;
StmtRef s0 = 0;
StmtRef sMinusOne = -1;

PKB generateParentTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::IfStmt);
	pkb.setStmtType(6, StmtType::Assign);
	pkb.setStmtType(7, StmtType::IfStmt);
	pkb.setStmtType(8, StmtType::IfStmt);
	pkb.setStmtType(9, StmtType::Read);
	return pkb;
}

TEST_CASE("Parent Methods") {
	PKB pkb = generateParentTestPKB();
	pkb.setStmtType(INT32_MAX, StmtType::Read);
	SECTION("Set Parent") {
		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setParent(s1, s2));
		CHECK_NOTHROW(pkb.setParent(s2, s3));

		// Parent statement can have multiple children
		CHECK_NOTHROW(pkb.setParent(s1, sIntMax));

		// Same Parent-child relation cannot be set twice.
		CHECK_THROWS(pkb.setParent(s1, s2));
		CHECK_THROWS(pkb.setParent(s2, s3));

		// A child cannot have multiple parents.
		CHECK_THROWS(pkb.setParent(s2, sIntMax));
		CHECK_THROWS(pkb.setParent(s1, s3));
		pkb = generateParentTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setParent(sMinusOne, s1));
		CHECK_THROWS(pkb.setParent(s2, s1));
		CHECK_THROWS(pkb.setParent(s3, s3));
		CHECK_THROWS(pkb.setParent(s0, s2));
		CHECK_THROWS(pkb.setParent(s3, sMinusOne));

		// StmtRef <= 0 not allowed for get methods.
		CHECK_THROWS(pkb.getParent(sMinusOne));
		CHECK_THROWS(pkb.getChildren(s0));
	}

	SECTION("Get Parent") {
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();
		pkb.setParent(s1, s2);
		pkb.setParent(s2, s3);
		pkb.setParent(s2, s4);

		// Ensure simple parent relation is stored successfully.
		CHECK(pkb.checkParents(s1, s2));
		CHECK(pkb.checkParents(s2, s3));
		CHECK(pkb.getParent(s2)->reference == s1);
		CHECK(pkb.getParent(s4)->reference == s2);
		pkb.getParent(s4);
		shared_ptr<StmtInfo> expected_s2_info = stmt_info_map.find(s2)->second;
		shared_ptr<StmtInfo> expected_s3_info = stmt_info_map.find(s3)->second;
		shared_ptr<StmtInfo> expected_s4_info = stmt_info_map.find(s4)->second;
		unordered_set<shared_ptr<StmtInfo>> expected_child_set_single = {expected_s2_info};
		CHECK(pkb.getChildren(s1).begin()->get()->reference == expected_child_set_single.begin()->get()->reference);

		unordered_set<shared_ptr<StmtInfo>> test_child_set_s2_raw = pkb.getChildren(s2);
		//	vector<StmtInfo> test_child_set_s2 = {};
		unordered_set<shared_ptr<StmtInfo>> expected_child_set_s2 = {expected_s3_info, expected_s4_info};
		CHECK(test_child_set_s2_raw == expected_child_set_s2);

		// Ensure Parent* behavior does not appear for simple parent.
		CHECK_FALSE(pkb.checkParents(s1, s3));

		// Test invalid check-parents cases.
		CHECK_FALSE(pkb.checkParents(s1, s1));
		CHECK_FALSE(pkb.checkParents(s2, s1));
		CHECK_FALSE(pkb.checkParents(s3, s2));
	}

	SECTION("Check Parent-child relationships") {
		pkb.setParent(s1, s2);
		pkb.setParent(s2, s3);

		CHECK(pkb.getParent(s1) == nullptr);  // Highest statement should have no parent.
		CHECK(pkb.getParent(s2)->reference == 1);
		CHECK(pkb.getParent(s3)->reference == 2);

		// Statement which was not stored in PKB.
		CHECK(pkb.getParent(sIntMax) == nullptr);
	}
}

TEST_CASE("ParentStar Methods") {
	PKB pkb = generateParentTestPKB();

	// Nesting levels:
	// 1--|
	//  2------>5--->7--|
	//    3->4-|  6-|  8--|
	//                   9--|
	pkb.setParent(s1, s2);
	pkb.setParent(s2, s3);
	pkb.setParent(s2, s4);
	pkb.setParent(s1, s5);
	pkb.setParent(s5, s6);
	pkb.setParent(s1, s7);
	pkb.setParent(s7, s8);
	pkb.setParent(s8, s9);

	pkb.populateComplexRelations();

	unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

	SECTION("Check Populate ParentStar Method") {
		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s1 = {};
		CHECK(pkb.getParentStar(s1) == expected_parent_star_s1);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s2 = {stmt_info_map.at(s1)};
		CHECK(pkb.getParentStar(s2) == expected_parent_star_s2);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s3 = {
			stmt_info_map.at(s1),
			stmt_info_map.at(s2)
		};
		CHECK(pkb.getParentStar(s3) == expected_parent_star_s3);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s4 = {
			stmt_info_map.at(s1),
			stmt_info_map.at(s2)
		};
		CHECK(pkb.getParentStar(s4) == expected_parent_star_s4);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s5	= {stmt_info_map.at(s1)};
		CHECK(pkb.getParentStar(s5) == expected_parent_star_s5);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s6 = {
			stmt_info_map.at(s1),
			stmt_info_map.at(s5)
		};
		CHECK(pkb.getParentStar(s6) == expected_parent_star_s6);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s7 = {
			stmt_info_map.at(s1)
		};
		CHECK(pkb.getParentStar(s7) == expected_parent_star_s7);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s8 = {
			stmt_info_map.at(s1),
			stmt_info_map.at(s7)
		};
		CHECK(pkb.getParentStar(s8) == expected_parent_star_s8);

		unordered_set<shared_ptr<StmtInfo>> expected_parent_star_s9 = {
			stmt_info_map.at(s1),
			stmt_info_map.at(s7),
			stmt_info_map.at(s8)
		};
		CHECK(pkb.getParentStar(s9) == expected_parent_star_s9);
	}

	SECTION("Check Populate ChildStar Method") {
		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s1 = {
			stmt_info_map.at(s2), stmt_info_map.at(s3), stmt_info_map.at(s4),
			stmt_info_map.at(s5), stmt_info_map.at(s6), stmt_info_map.at(s7),
			stmt_info_map.at(s8), stmt_info_map.at(s9),
		};
		CHECK(pkb.getChildStar(s1) == expected_child_star_s1);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s2 = {
			stmt_info_map.at(s3), stmt_info_map.at(s4)
		};
		CHECK(pkb.getChildStar(s2) == expected_child_star_s2);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s3 = {};
		CHECK(pkb.getChildStar(s3) == expected_child_star_s3);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s4 = {};
		CHECK(pkb.getChildStar(s4) == expected_child_star_s4);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s5 = {stmt_info_map.at(s6)};
		CHECK(pkb.getChildStar(s5) == expected_child_star_s5);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s6 = {};
		CHECK(pkb.getChildStar(s6) == expected_child_star_s6);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s7 = {
			stmt_info_map.at(s8), stmt_info_map.at(s9)
		};
		CHECK(pkb.getChildStar(s7) == expected_child_star_s7);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s8 = {stmt_info_map.at(s9)};
		CHECK(pkb.getChildStar(s8) == expected_child_star_s8);

		unordered_set<shared_ptr<StmtInfo>> expected_child_star_s9 = {};
		CHECK(pkb.getChildStar(s9) == expected_child_star_s9);
	}
}
