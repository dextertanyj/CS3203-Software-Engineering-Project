#include "PKB/ParentRelation.h"

#include "TestUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::ParentRelation::insertForward Test") {
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(3, StmtType::Read));
	REQUIRE_NOTHROW(relation.insertForward(TestUtilities::createStmtInfo(2, StmtType::Print)));
}

TEST_CASE("PKB::ParentRelation::insertReverse Test") {
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(2, StmtType::Read));
	REQUIRE_NOTHROW(relation.insertReverse(TestUtilities::createStmtInfo(3, StmtType::Print)));
}

TEST_CASE("PKB::ParentRelation::appendReverseTransitive Test") {
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(5, StmtType::Print);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(7, StmtType::If);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(1, StmtType::Print);
	unordered_set<shared_ptr<StmtInfo>> set({s1, s2, s3});
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(4, StmtType::Read));
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set));
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
}

TEST_CASE("PKB::ParentRelation::appendForwardTransitive Test") {
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(3, StmtType::If);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(7, StmtType::Print);
	unordered_set<shared_ptr<StmtInfo>> set({s1, s2, s3});
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(6, StmtType::Read));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
}

TEST_CASE("PKB::ParentRelation::getForward Test") {
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(3, StmtType::Read));
	REQUIRE_EQUALS(relation.getForward(), unordered_set<shared_ptr<StmtInfo>>());
	shared_ptr<StmtInfo> node = TestUtilities::createStmtInfo(2, StmtType::Print);
	REQUIRE_NOTHROW(relation.insertForward(node));
	REQUIRE_EQUALS(relation.getForward().size(), 1);
	REQUIRE(relation.getForward().find(node) != relation.getForward().end());
}

TEST_CASE("PKB::ParentRelation::getReverse Test") {
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(2, StmtType::Read));
	REQUIRE_EQUALS(relation.getReverse(), unordered_set<shared_ptr<StmtInfo>>());
	shared_ptr<StmtInfo> node = TestUtilities::createStmtInfo(3, StmtType::Print);
	REQUIRE_NOTHROW(relation.insertReverse(node));
	REQUIRE_EQUALS(relation.getReverse().size(), 1);
	REQUIRE(relation.getReverse().find(node) != relation.getReverse().end());
}

TEST_CASE("PKB::ParentRelation::getForwardTransitive Test") {
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(3, StmtType::If);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(7, StmtType::Print);
	unordered_set<shared_ptr<StmtInfo>> set({s1, s2, s3});
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(6, StmtType::Read));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
}

TEST_CASE("PKB::ParentRelation::getReverseTransitive Test") {
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(5, StmtType::If);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	unordered_set<shared_ptr<StmtInfo>> set({s1, s2, s3});
	PKB::ParentRelation relation = PKB::ParentRelation(TestUtilities::createStmtInfo(2, StmtType::Read));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set2));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
}

TEST_CASE("PKB::ParentRelation Overall Test") {
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(4, StmtType::If);
	shared_ptr<StmtInfo> s4 = TestUtilities::createStmtInfo(7, StmtType::Call);
	unordered_set<shared_ptr<StmtInfo>> forward({s1, s2});
	unordered_set<shared_ptr<StmtInfo>> reverse({s3, s4});
	shared_ptr<StmtInfo> self = TestUtilities::createStmtInfo(3, StmtType::Read);
	PKB::ParentRelation relation = PKB::ParentRelation(self);
	REQUIRE_NOTHROW(relation.insertForward(s2));
	REQUIRE_EQUALS(relation.getForward().size(), 1);
	REQUIRE(relation.getForward().find(s2) != relation.getForward().end());
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendForwardTransitive(forward));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 2);
	REQUIRE(relation.getForwardTransitive().find(s1) != relation.getForwardTransitive().end());
	REQUIRE(relation.getForwardTransitive().find(s2) != relation.getForwardTransitive().end());
	REQUIRE_NOTHROW(relation.insertReverse(s3));
	REQUIRE_EQUALS(relation.getReverse().size(), 1);
	REQUIRE(relation.getReverse().find(s3) != relation.getReverse().end());
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendReverseTransitive(reverse));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 2);
	REQUIRE(relation.getReverseTransitive().find(s3) != relation.getReverseTransitive().end());
	REQUIRE(relation.getReverseTransitive().find(s4) != relation.getReverseTransitive().end());
}

TEST_CASE("PKB::ParentRelation::optimize Test") {
	PKB::Types::ParentStore store = PKB::Types::ParentStore();
	shared_ptr<StmtInfo> s1 = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> s2 = TestUtilities::createStmtInfo(7, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = TestUtilities::createStmtInfo(3, StmtType::If);
	shared_ptr<StmtInfo> s1_if = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> s2_if = TestUtilities::createStmtInfo(5, StmtType::Print);
	shared_ptr<StmtInfo> s3_if = TestUtilities::createStmtInfo(6, StmtType::Assign);
	store.set(s1, s2);
	store.set(s1, s3);
	store.set(s3, s1_if);
	store.set(s3, s2_if);
	store.set(s3, s3_if);
	REQUIRE(find(store.getReverse(2).begin(), store.getReverse(2).end(), s3) != store.getReverse(2).end());
	REQUIRE(find(store.getForward(3).begin(), store.getForward(3).end(), s1) != store.getForward(3).end());
	REQUIRE_EQUALS(store.getReverseTransitive(2).size(), 0);
	REQUIRE_NOTHROW(store.optimize());
	REQUIRE_EQUALS(store.getReverseTransitive(2).size(), 5);
	REQUIRE_EQUALS(store.getForwardTransitive(4).size(), 2);
	REQUIRE(find(store.getForwardTransitive(4).begin(), store.getForwardTransitive(4).end(), s3) != store.getForwardTransitive(4).end());
	REQUIRE(find(store.getForwardTransitive(4).begin(), store.getForwardTransitive(4).end(), s1) != store.getForwardTransitive(4).end());
	REQUIRE_EQUALS(store.getForwardTransitive(7).size(), 1);
	REQUIRE(find(store.getForwardTransitive(7).begin(), store.getForwardTransitive(7).end(), s1) != store.getForwardTransitive(7).end());
	REQUIRE_EQUALS(store.getReverseTransitive(7).size(), 0);
}
