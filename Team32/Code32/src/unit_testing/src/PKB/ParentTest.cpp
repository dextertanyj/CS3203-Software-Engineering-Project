#include "PKB/PKB.h"
#include "catch.hpp"
#include "memory"

TEST_CASE("Parents Methods") {
	StmtInfo stmt1 = {1, StmtType::WhileStmt};
	StmtInfo stmt2 = {2, StmtType::IfStmt};
	StmtInfo stmt3 = {3, StmtType::Print};
	StmtInfo stmtIntMax = {INT32_MAX, StmtType::Read};
	StmtInfo stmtNegative = {-1, StmtType::Assign};
	StmtInfo stmt0 = {0, StmtType::Read};

	auto stmt1Ptr = make_shared<StmtInfo>(stmt1);
	auto stmt2Ptr = make_shared<StmtInfo>(stmt2);
	auto stmt3Ptr = make_shared<StmtInfo>(stmt3);
	auto stmtIntMaxPtr = make_shared<StmtInfo>(stmtIntMax);
	auto stmtNegativePtr = make_shared<StmtInfo>(stmtNegative);
	auto stmt0Ptr = make_shared<StmtInfo>(stmt0);

	ParentStore ps = ParentStore();

	SECTION("Check validity of arguments") {
		// Verify that normal setting works.
		CHECK_NOTHROW(ps.setParent(stmt1Ptr, stmt2Ptr));
		CHECK_NOTHROW(ps.setParent(stmt2Ptr, stmt3Ptr));
		CHECK_NOTHROW(ps.setParent(stmt1Ptr, stmtIntMaxPtr));
		ps.clear();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(ps.setParent(stmtNegativePtr, stmt1Ptr));
		CHECK_THROWS(ps.setParent(stmt2Ptr, stmt1Ptr));
		CHECK_THROWS(ps.setParent(stmt3Ptr, stmt3Ptr));
		CHECK_THROWS(ps.setParent(stmt0Ptr, stmt2Ptr));
		CHECK_THROWS(ps.setParent(stmt3Ptr, stmtNegativePtr));
		CHECK_THROWS(ps.getParent(stmtNegativePtr));
		CHECK_THROWS(ps.getChildren(stmt0Ptr));
	}

	SECTION("Check seting and getting of parent/child") {
		StmtInfo s4 = {4, StmtType::Print};
		shared_ptr<StmtInfo> stmt4Ptr = make_shared<StmtInfo>(s4);

		ps.setParent(stmt1Ptr, stmt2Ptr);
		ps.setParent(stmt2Ptr, stmt3Ptr);
		ps.setParent(stmt2Ptr, stmt4Ptr);

		// Ensure simple parent relation is stored successfully.
		CHECK(ps.isParentChild(stmt1Ptr, stmt2Ptr));
		CHECK(ps.isParentChild(stmt2Ptr, stmt3Ptr));
		CHECK(ps.getParent(stmt2Ptr) == stmt1Ptr);
		CHECK(ps.getParent(stmt4Ptr) == stmt2Ptr);
		CHECK(ps.getChildren(stmt1Ptr) == unordered_set<shared_ptr<StmtInfo>>{stmt2Ptr});
		CHECK(ps.getChildren(stmt2Ptr) == unordered_set<shared_ptr<StmtInfo>>{stmt3Ptr, stmt4Ptr});

		// Ensure Parent* behavior does not appear for simple parent.
		CHECK_FALSE(ps.isParentChild(stmt1Ptr, stmt3Ptr));

		CHECK_FALSE(ps.isParentChild(stmt1Ptr, stmt1Ptr));
		CHECK_FALSE(ps.isParentChild(stmt2Ptr, stmt1Ptr));
		CHECK_FALSE(ps.isParentChild(stmt3Ptr, stmt2Ptr));
	}

	SECTION("Get Parent") {
		ParentStore ps = ParentStore();

		ps.setParent(stmt1Ptr, stmt2Ptr);
		ps.setParent(stmt2Ptr, stmt3Ptr);

		CHECK(ps.getParent(stmt1Ptr)->reference == -1);  // Highest statement should have no parent.
		CHECK(ps.getParent(stmt2Ptr)->reference == 1);
		CHECK(ps.getParent(stmt3Ptr)->reference == 2);

		// Statement which was not stored in PKB.
		CHECK(ps.getParent(stmtIntMaxPtr)->reference == -1);
	}
}

	// TODO: Test Parent* functionality.