#include "catch.hpp"

#include "../../../spa/src/PKB/FollowStore.cpp"
#include "../../../spa/src/Common/TypeDefs.h"

TEST_CASE("Set Follows") {
  StmtRef s1 = 1;
  StmtRef s2 = 2;
  StmtRef s3 = 3;
  FollowStore fs = FollowStore();

  // Verify that normal setting works.  
  CHECK_NOTHROW(fs.setFollows(s1,s2));
  CHECK_NOTHROW(fs.setFollows(s2,s3));
  CHECK_NOTHROW(fs.setFollows(1, INT32_MAX));

  // Verify that improper arguments lead to an exception thrown.
  CHECK_THROWS(fs.setFollows(-1, 1));
  CHECK_THROWS(fs.setFollows(2, 1));
  CHECK_THROWS(fs.setFollows(3, 3));
  CHECK_THROWS(fs.setFollows(0, 2));
  CHECK_THROWS(fs.setFollows(3, -4));  
}

TEST_CASE( "Basic Follows Set and Get") {
  StmtRef s1 = 1;
  StmtRef s2 = 2;
  StmtRef s3 = 3;
  FollowStore fs = FollowStore();
  
  fs.setFollows(s1, s2);
  fs.setFollows(s2, s3);

  // Ensure simple follow relation is stored successfully.
  CHECK( fs.checkFollows(s1, s2));
  CHECK( fs.checkFollows(s2, s3));

  // Ensure Follows* behavior does not appear for simple follow.
  CHECK_FALSE( fs.checkFollows(s1, s3));

  CHECK_FALSE(fs.checkFollows(s1, s1));
  CHECK_FALSE(fs.checkFollows(s2, s1));
  CHECK_FALSE(fs.checkFollows(s3, s2));
  
}

// TODO: Test Follow* functionality.