#include "catch.hpp"
#include "memory"
#include "PKB/ParentStore.cpp"

TEST_CASE( "Basic Parent Set and Get") {
  StmtRef s1 = 1;
  StmtRef s2 = 2;
  StmtRef s3 = 3;
  ParentStore ps = ParentStore();
  
  ps.setParent(s1, s2);
  ps.setParent(s2, s3);

  // Ensure simple parent relation is stored successfully.
  CHECK( ps.isParentChild(s1, s2));
  CHECK( ps.isParentChild(s2, s3));

  // Ensure Parent* behavior does not appear for simple parent.
  CHECK_FALSE( ps.isParentChild(s1, s3));
}
TEST_CASE("Parents Methods") {
	StmtRef s1 = 1;
	StmtRef s2 = 2;
	StmtRef s3 = 3;
	ParentStore ps = ParentStore();

	SECTION("Check validity of arguments") {
		// Verify that normal setting works.
		CHECK_NOTHROW(ps.setParent(s1,s2));
		CHECK_NOTHROW(ps.setParent(s2,s3));
		CHECK_NOTHROW(ps.setParent(1, INT32_MAX));

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(ps.setParent(-1, 1));
		CHECK_THROWS(ps.setParent(2, 1));
		CHECK_THROWS(ps.setParent(3, 3));
		CHECK_THROWS(ps.setParent(0, 2));
		CHECK_THROWS(ps.setParent(3, -4));
		CHECK_THROWS(ps.getParent(-1));
		CHECK_THROWS(ps.getChildren(0));
	}

	SECTION("Basic set and get parents") {
		StmtRef s4 = 4;

		ps.setParent(s1, s2);
		ps.setParent(s2, s3);
		ps.setParent(s2, s4);

		// Ensure simple parent relation is stored successfully.
		CHECK( ps.checkParents(s1, s2));
		CHECK( ps.checkParents(s2, s3));
		CHECK( ps.getParent(s2) == s1);
		CHECK( ps.getParent(s4) == s2);
		CHECK (ps.getChildren(s1) == unordered_set<StmtRef>{ s2 });
		CHECK (ps.getChildren(s2) == unordered_set<StmtRef>{ s3, s4 });

		// Ensure Parent* behavior does not appear for simple parent.
		CHECK_FALSE( ps.checkParents(s1, s3));

		CHECK_FALSE(ps.checkParents(s1, s1));
		CHECK_FALSE(ps.checkParents(s2, s1));
		CHECK_FALSE(ps.checkParents(s3, s2));
	}
TEST_CASE("Get Parent") {
  StmtRef s1 = 1;
  StmtRef s2 = 2;
  StmtRef s3 = 3;
  ParentStore ps = ParentStore();

  ps.setParent(s1, s2);
  ps.setParent(s2, s3);

  CHECK(ps.getParent(s1) == -1); // Highest statement should have no parent.
  CHECK(ps.getParent(s2) == s1);
  CHECK(ps.getParent(s3) == s2);

  // Statement which was not stored in PKB.
  CHECK(ps.getParent(123) == -1);
}

// TODO: Test Parent* functionality.