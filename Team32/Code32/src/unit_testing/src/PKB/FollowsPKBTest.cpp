#include "PKB/FollowsPKB.h"

#include "MockUtilities.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "catch_tools.h"
//
//TEST_CASE("PKB::FollowsPKB::insertForward Test") {
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(1, StmtType::Read));
//	REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(3, StmtType::Print)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(2, StmtType::Assign)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(3, StmtType::Assign)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(3, StmtType::Read)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertForward(nullptr), std::exception);
//}
//
//TEST_CASE("PKB::FollowsPKB::insertReverse Test") {
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(3, StmtType::Read));
//    REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Print)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(2, StmtType::Assign)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Read)), std::exception);
//    REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Print)), std::exception);
//}
//
//TEST_CASE("PKB::FollowsPKB::appendForwardTransitive Test") {
//    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(3, StmtType::Print);
//    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(7, StmtType::Assign);
//    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(4, StmtType::IfStmt);
//    unordered_set<shared_ptr<StmtInfo>> set ({s1, s2, s3 });
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(1, StmtType::Read));
//    REQUIRE_THROWS_AS(pkb.appendForwardTransitive(set), std::exception);
//    // add empty set
//    unordered_set<shared_ptr<StmtInfo>> set2 ;
//    pkb.appendForwardTransitive(set2);
//}
//
//TEST_CASE("PKB::FollowsPKB::appendReverseTransitive Test") {
//    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(2, StmtType::Print);
//    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(3, StmtType::Assign);
//    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(4, StmtType::IfStmt);
//    unordered_set<shared_ptr<StmtInfo>> set ({s1, s2, s3 });
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(7, StmtType::Read));
//    pkb.appendReverseTransitive(set);
//    // add empty set
//    unordered_set<shared_ptr<StmtInfo>> set2 ;
//    REQUIRE_THROWS(pkb.appendReverseTransitive(set2));
//}
//
//TEST_CASE("PKB::FollowsPKB::getForward Test") {
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(1, StmtType::Read));
//    REQUIRE_EQUALS(pkb.getForward(), unordered_set<shared_ptr<StmtInfo>>());
//    shared_ptr<StmtInfo> node = MockUtilities::createStmtInfo(3, StmtType::Print);
//    pkb.insertForward(node);
//    REQUIRE_EQUALS(pkb.getForward().size(), 1);
//    REQUIRE(pkb.getForward().find(node) != pkb.getForward().end());
//}
//
//TEST_CASE("PKB::FollowsPKB::getReverse Test") {
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(3, StmtType::Read));
//    REQUIRE_EQUALS(pkb.getReverse(), unordered_set<shared_ptr<StmtInfo>>());
//    shared_ptr<StmtInfo> node = MockUtilities::createStmtInfo(1, StmtType::Print);
//    pkb.insertReverse(node);
//    REQUIRE_EQUALS(pkb.getReverse().size(), 1);
//    REQUIRE(pkb.getReverse().find(node) != pkb.getReverse().end());
//}
//
//TEST_CASE("PKB::FollowsPKB::getForwardTransitive Test") {
//    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(3, StmtType::Print);
//    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(7, StmtType::Assign);
//    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(4, StmtType::IfStmt);
//    shared_ptr<StmtInfo> s3_dup = MockUtilities::createStmtInfo(4, StmtType::Assign);
//    unordered_set<shared_ptr<StmtInfo>> set ({s1, s2, s3 });
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(2, StmtType::Read));
//    pkb.appendForwardTransitive(set);
//    REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 3);
//
//    // TODO: CHECK
//    pkb.appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>>{s3_dup});
//    REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 4);
//
//    shared_ptr<StmtInfo> before_curr_s = MockUtilities::createStmtInfo(1, StmtType::Assign);
//    pkb.appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>>{before_curr_s});
//    REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 5);
//    REQUIRE(pkb.getForwardTransitive().find(before_curr_s) != pkb.getForwardTransitive().end());
//}
//
//TEST_CASE("PKB::FollowsPKB::getReverseTransitive Test") {
//    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(2, StmtType::Print);
//    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(3, StmtType::Assign);
//    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(4, StmtType::IfStmt);
//    shared_ptr<StmtInfo> s3_dup = MockUtilities::createStmtInfo(4, StmtType::Assign);
//    unordered_set<shared_ptr<StmtInfo>> set ({s1, s2, s3 });
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(7, StmtType::Read));
//    pkb.appendReverseTransitive(set);
//    REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 3);
//
//    // TODO: CHECK
//    pkb.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>{s3_dup});
//    REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 4);
//}
//
//TEST_CASE("PKB::FollowsPKB Overall Test") {
//    shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Print);
//    shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
//    shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(5, StmtType::IfStmt);
//    unordered_set<shared_ptr<StmtInfo>> set_before ({s1, s2 });
//    unordered_set<shared_ptr<StmtInfo>> set_after ({ s3 });
//    FollowsPKB pkb = FollowsPKB(MockUtilities::createStmtInfo(4, StmtType::Read));
//    pkb.insertForward(s3);
//    REQUIRE_EQUALS(pkb.getForward().size(), 1);
//    REQUIRE(pkb.getForward().find(s3) != pkb.getForward().end());
//    REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 0);
//    REQUIRE_FALSE(pkb.getForwardTransitive().find(s3) != pkb.getForwardTransitive().end());
//    pkb.appendForwardTransitive(set_after);
//    REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 1);
//    REQUIRE(pkb.getForwardTransitive().find(s3) != pkb.getForwardTransitive().end());
//    pkb.insertReverse(s2);
//    REQUIRE_EQUALS(pkb.getReverse().size(), 1);
//    REQUIRE(pkb.getReverse().find(s2) != pkb.getReverse().end());
//    REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 0);
//    REQUIRE_FALSE(pkb.getReverseTransitive().find(s2) != pkb.getReverseTransitive().end());
//    pkb.appendReverseTransitive(set_before);
//    REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 2);
//    REQUIRE(pkb.getReverseTransitive().find(s2) != pkb.getReverseTransitive().end());
//    REQUIRE(pkb.getReverseTransitive().find(s1) != pkb.getReverseTransitive().end());
//}

// TODO: IMPLEMENT OPTIMISE TEST