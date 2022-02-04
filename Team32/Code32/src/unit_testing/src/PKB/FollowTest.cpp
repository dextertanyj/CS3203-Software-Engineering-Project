#include "catch.hpp"

#include "../../../spa/src/PKB/FollowStore.cpp"

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
}

// TODO: Test Follow* functionality.