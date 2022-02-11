#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "memory"

PKB generateParentTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(INT32_MAX, StmtType::Read);
	return pkb;
}

StmtRef s1 = 1;
StmtRef s2 = 2;
StmtRef s3 = 3;
StmtRef s4 = 4;
StmtRef sIntMax = INT32_MAX;
StmtRef s0 = 0;
StmtRef sMinusOne = -1;

TEST_CASE("Parent Methods") {
	SECTION("Set Parent") {
		PKB pkb = generateParentTestPKB();
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
		PKB pkb = generateParentTestPKB();
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();
		pkb.setParent(s1, s2);
		pkb.setParent(s2, s3);
		pkb.setParent(s2, s4);

		// Ensure simple parent relation is stored successfully.
		CHECK(pkb.checkParents(s1, s2));
		CHECK(pkb.checkParents(s2, s3));
		CHECK(pkb.getParent(s2)->reference == s1);
		CHECK(pkb.getParent(s4)->reference == s2);
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
		PKB pkb = generateParentTestPKB();
		pkb.setParent(s1, s2);
		pkb.setParent(s2, s3);

		CHECK(pkb.getParent(s1)->reference == -1);  // Highest statement should have no parent.
		CHECK(pkb.getParent(s2)->reference == 1);
		CHECK(pkb.getParent(s3)->reference == 2);

		// Statement which was not stored in PKB.
		CHECK(pkb.getParent(sIntMax)->reference == -1);
	}
}
// TODO: Test Parent* functionality.