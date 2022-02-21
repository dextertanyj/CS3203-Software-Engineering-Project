#include "MockUtilities.h"
#include "PKB/FollowsRelation.h"
#include "PKB/Storage.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::FollowsRelation::insertForward Test") {
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(3, StmtType::Read));
	REQUIRE_NOTHROW(relation.insertForward(MockUtilities::createStmtInfo(1, StmtType::Read)));
	REQUIRE_THROWS_AS(relation.insertForward(MockUtilities::createStmtInfo(3, StmtType::Print)), std::exception);
	REQUIRE_THROWS_AS(relation.insertForward(MockUtilities::createStmtInfo(4, StmtType::Assign)), std::exception);
	REQUIRE_THROWS_AS(relation.insertForward(MockUtilities::createStmtInfo(7, StmtType::Assign)), std::exception);
	REQUIRE_THROWS_AS(relation.insertForward(MockUtilities::createStmtInfo(1, StmtType::Read)), std::exception);
}

TEST_CASE("PKB::FollowsRelation::insertReverse Test") {
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(3, StmtType::Read));
	REQUIRE_NOTHROW(relation.insertReverse(MockUtilities::createStmtInfo(4, StmtType::Print)));
	REQUIRE_THROWS_AS(relation.insertReverse(MockUtilities::createStmtInfo(3, StmtType::Print)), std::exception);
	REQUIRE_THROWS_AS(relation.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Print)), std::exception);
	REQUIRE_THROWS_AS(relation.insertReverse(MockUtilities::createStmtInfo(2, StmtType::Assign)), std::exception);
	REQUIRE_THROWS_AS(relation.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Read)), std::exception);
	REQUIRE_THROWS_AS(relation.insertReverse(MockUtilities::createStmtInfo(4, StmtType::Read)), std::exception);
}

TEST_CASE("PKB::FollowsRelation::appendReverseTransitive Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(5, StmtType::Print);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(7, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(1, StmtType::Print);
	unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(4, StmtType::Read));
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set));
	REQUIRE_THROWS_AS(relation.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
}

TEST_CASE("PKB::FollowsRelation::appendForwardTransitive Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(7, StmtType::Print);
	unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(6, StmtType::Read));
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set));
	REQUIRE_THROWS_AS(relation.appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
}

TEST_CASE("PKB::FollowsRelation::getForward Test") {
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(3, StmtType::Read));
	REQUIRE_EQUALS(relation.getForward(), unordered_set<shared_ptr<StmtInfo>>());
	shared_ptr<StmtInfo> node = MockUtilities::createStmtInfo(2, StmtType::Print);
	REQUIRE_NOTHROW(relation.insertForward(node));
	REQUIRE_EQUALS(relation.getForward().size(), 1);
	REQUIRE(relation.getForward().find(node) != relation.getForward().end());
}

TEST_CASE("PKB::FollowsRelation::getReverse Test") {
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(2, StmtType::Read));
	REQUIRE_EQUALS(relation.getReverse(), unordered_set<shared_ptr<StmtInfo>>());
	shared_ptr<StmtInfo> node = MockUtilities::createStmtInfo(3, StmtType::Print);
	REQUIRE_NOTHROW(relation.insertReverse(node));
	REQUIRE_EQUALS(relation.getReverse().size(), 1);
	REQUIRE(relation.getReverse().find(node) != relation.getReverse().end());
}

TEST_CASE("PKB::FollowsRelation::getForwardTransitive Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(7, StmtType::Print);
	unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(6, StmtType::Read));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	REQUIRE_THROWS_AS(relation.appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
}

TEST_CASE("PKB::FollowsRelation::getReverseTransitive Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(4, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(5, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(1, StmtType::Assign);
	unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
	PKB::FollowsRelation relation = PKB::FollowsRelation(MockUtilities::createStmtInfo(2, StmtType::Read));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
	REQUIRE_THROWS_AS(relation.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
	unordered_set<shared_ptr<StmtInfo>> set2;
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set2));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
}

TEST_CASE("PKB::FollowsRelation Overall Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(4, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(7, StmtType::Call);
	unordered_set<shared_ptr<StmtInfo>> forward({s_1, s_2});
	unordered_set<shared_ptr<StmtInfo>> reverse({s_3, s_4});
	shared_ptr<StmtInfo> self = MockUtilities::createStmtInfo(3, StmtType::Read);
	PKB::FollowsRelation relation = PKB::FollowsRelation(self);
	REQUIRE_NOTHROW(relation.insertForward(s_2));
	REQUIRE_EQUALS(relation.getForward().size(), 1);
	REQUIRE(relation.getForward().find(s_2) != relation.getForward().end());
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendForwardTransitive(forward));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 2);
	REQUIRE(relation.getForwardTransitive().find(s_1) != relation.getForwardTransitive().end());
	REQUIRE(relation.getForwardTransitive().find(s_2) != relation.getForwardTransitive().end());
	REQUIRE_NOTHROW(relation.insertReverse(s_3));
	REQUIRE_EQUALS(relation.getReverse().size(), 1);
	REQUIRE(relation.getReverse().find(s_3) != relation.getReverse().end());
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendReverseTransitive(reverse));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 2);
	REQUIRE(relation.getReverseTransitive().find(s_3) != relation.getReverseTransitive().end());
	REQUIRE(relation.getReverseTransitive().find(s_4) != relation.getReverseTransitive().end());
	REQUIRE_THROWS_AS(relation.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>({self})), std::exception);
}

TEST_CASE("PKB::FollowsRelation::optimize Test") {
	PKB::StatementRelationStore<PKB::FollowsRelation> store = PKB::StatementRelationStore<PKB::FollowsRelation>();
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Call);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2_1 = MockUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> s_2_2 = MockUtilities::createStmtInfo(4, StmtType::Print);
	shared_ptr<StmtInfo> s_2_3 = MockUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(6, StmtType::IfStmt);
	store.set(s_1, s_2);
	store.set(s_2, s_3);
	store.set(s_2_1, s_2_2);
	store.set(s_2_2, s_2_3);
	REQUIRE_EQUALS(store.getReverse(2).size(), 1);
	REQUIRE(find(store.getReverse(2).begin(), store.getReverse(2).end(), s_3) != store.getReverse(2).end());
	REQUIRE_EQUALS(store.getForward(2).size(), 1);
	REQUIRE(find(store.getForward(2).begin(), store.getForward(2).end(), s_1) != store.getForward(2).end());
	REQUIRE_EQUALS(store.getForwardTransitive(2).size(), 0);
	REQUIRE_EQUALS(store.getReverseTransitive(2).size(), 0);
	REQUIRE_NOTHROW(PKB::FollowsRelation::optimize(store));
	REQUIRE_EQUALS(store.getForwardTransitive(6).size(), 2);
	REQUIRE(find(store.getForwardTransitive(6).begin(), store.getForwardTransitive(6).end(), s_2) != store.getForwardTransitive(6).end());
	REQUIRE(find(store.getForwardTransitive(6).begin(), store.getForwardTransitive(6).end(), s_1) != store.getForwardTransitive(6).end());
	REQUIRE_EQUALS(store.getReverseTransitive(1).size(), 2);
	REQUIRE(find(store.getReverseTransitive(1).begin(), store.getReverseTransitive(1).end(), s_2) != store.getReverseTransitive(1).end());
	REQUIRE(find(store.getReverseTransitive(1).begin(), store.getReverseTransitive(1).end(), s_3) != store.getReverseTransitive(1).end());
}
