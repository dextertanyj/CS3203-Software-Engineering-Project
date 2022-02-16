#include "MockUtilities.h"
#include "PKB/FollowsRelation.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::FollowsRelation Internal Methods Test") {
	SECTION("PKB::FollowsRelation::insertForward Test") {
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(3, StmtType::Read));
		pkb.insertForward(MockUtilities::createStmtInfo(1, StmtType::Read));
		REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(3, StmtType::Print)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(4, StmtType::Assign)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(7, StmtType::Assign)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertForward(MockUtilities::createStmtInfo(1, StmtType::Read)), std::exception);
	}

	SECTION("PKB::FollowsRelation::insertReverse Test") {
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(3, StmtType::Read));
		pkb.insertReverse(MockUtilities::createStmtInfo(4, StmtType::Print));
		REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(3, StmtType::Print)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Print)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(2, StmtType::Assign)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(1, StmtType::Read)), std::exception);
		REQUIRE_THROWS_AS(pkb.insertReverse(MockUtilities::createStmtInfo(4, StmtType::Read)), std::exception);
	}

	SECTION("PKB::FollowsRelation::appendReverseTransitive Test") {
		shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(5, StmtType::Print);
		shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(6, StmtType::Assign);
		shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(7, StmtType::IfStmt);
		shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(1, StmtType::Print);
		unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(4, StmtType::Read));
		pkb.appendReverseTransitive(set);
		REQUIRE_THROWS_AS(pkb.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
		unordered_set<shared_ptr<StmtInfo>> set2;
		pkb.appendForwardTransitive(set2);
	}

	SECTION("PKB::FollowsRelation::appendForwardTransitive Test") {
		shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Print);
		shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
		shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
		shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(7, StmtType::Print);
		unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(6, StmtType::Read));
		pkb.appendForwardTransitive(set);
		REQUIRE_THROWS_AS(pkb.appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
		unordered_set<shared_ptr<StmtInfo>> set2;
		pkb.appendForwardTransitive(set2);
	}

	SECTION("PKB::FollowsRelation::getForward Test") {
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(3, StmtType::Read));
		REQUIRE_EQUALS(pkb.getForward(), unordered_set<shared_ptr<StmtInfo>>());
		shared_ptr<StmtInfo> node = MockUtilities::createStmtInfo(2, StmtType::Print);
		pkb.insertForward(node);
		REQUIRE_EQUALS(pkb.getForward().size(), 1);
		REQUIRE(pkb.getForward().find(node) != pkb.getForward().end());
	}

	SECTION("PKB::FollowsRelation::getReverse Test") {
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(2, StmtType::Read));
		REQUIRE_EQUALS(pkb.getReverse(), unordered_set<shared_ptr<StmtInfo>>());
		shared_ptr<StmtInfo> node = MockUtilities::createStmtInfo(3, StmtType::Print);
		pkb.insertReverse(node);
		REQUIRE_EQUALS(pkb.getReverse().size(), 1);
		REQUIRE(pkb.getReverse().find(node) != pkb.getReverse().end());
	}

	SECTION("PKB::FollowsRelation::getForwardTransitive Test") {
		shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Print);
		shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
		shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
		shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(7, StmtType::Print);
		unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(6, StmtType::Read));
		REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 0);
		pkb.appendForwardTransitive(set);
		REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 3);
		REQUIRE_THROWS_AS(pkb.appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
		REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 3);
		unordered_set<shared_ptr<StmtInfo>> set2;
		pkb.appendForwardTransitive(set2);
		REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 3);
	}

	SECTION("PKB::FollowsRelation::getReverseTransitive Test") {
		shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(3, StmtType::Print);
		shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(4, StmtType::Assign);
		shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(5, StmtType::IfStmt);
		shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(1, StmtType::Assign);
		unordered_set<shared_ptr<StmtInfo>> set({s_1, s_2, s_3});
		FollowsRelation pkb = FollowsRelation(MockUtilities::createStmtInfo(2, StmtType::Read));
		REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 0);
		pkb.appendReverseTransitive(set);
		REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 3);
		REQUIRE_THROWS_AS(pkb.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>({s_4})), std::exception);
		REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 3);
		unordered_set<shared_ptr<StmtInfo>> set2;
		pkb.appendReverseTransitive(set2);
		REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 3);
	}

	SECTION("PKB::FollowsRelation Overall Test") {
		shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::Print);
		shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
		shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(4, StmtType::IfStmt);
		shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(7, StmtType::Call);
		unordered_set<shared_ptr<StmtInfo>> forward({s_1, s_2});
		unordered_set<shared_ptr<StmtInfo>> reverse({s_3, s_4});
		shared_ptr<StmtInfo> self = MockUtilities::createStmtInfo(3, StmtType::Read);
		FollowsRelation pkb = FollowsRelation(self);
		pkb.insertForward(s_2);
		REQUIRE_EQUALS(pkb.getForward().size(), 1);
		REQUIRE(pkb.getForward().find(s_2) != pkb.getForward().end());
		REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 0);
		pkb.appendForwardTransitive(forward);
		REQUIRE_EQUALS(pkb.getForwardTransitive().size(), 2);
		REQUIRE(pkb.getForwardTransitive().find(s_1) != pkb.getForwardTransitive().end());
		REQUIRE(pkb.getForwardTransitive().find(s_2) != pkb.getForwardTransitive().end());
		pkb.insertReverse(s_3);
		REQUIRE_EQUALS(pkb.getReverse().size(), 1);
		REQUIRE(pkb.getReverse().find(s_3) != pkb.getReverse().end());
		REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 0);
		pkb.appendReverseTransitive(reverse);
		REQUIRE_EQUALS(pkb.getReverseTransitive().size(), 2);
		REQUIRE(pkb.getReverseTransitive().find(s_3) != pkb.getReverseTransitive().end());
		REQUIRE(pkb.getReverseTransitive().find(s_4) != pkb.getReverseTransitive().end());
		REQUIRE_THROWS_AS(pkb.appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>>({self})), std::exception);
	}

	SECTION("PKB::FollowsRelation::optimize Test") {
		StatementRelationStore<FollowsRelation> store = StatementRelationStore<FollowsRelation>();
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
		FollowsRelation::optimize(store);
		REQUIRE_EQUALS(store.getForwardTransitive(6).size(), 2);
		REQUIRE(find(store.getForwardTransitive(6).begin(), store.getForwardTransitive(6).end(), s_2) !=
		        store.getForwardTransitive(6).end());
		REQUIRE(find(store.getForwardTransitive(6).begin(), store.getForwardTransitive(6).end(), s_1) !=
		        store.getForwardTransitive(6).end());
		REQUIRE_EQUALS(store.getReverseTransitive(1).size(), 2);
		REQUIRE(find(store.getReverseTransitive(1).begin(), store.getReverseTransitive(1).end(), s_2) !=
		        store.getReverseTransitive(1).end());
		REQUIRE(find(store.getReverseTransitive(1).begin(), store.getReverseTransitive(1).end(), s_3) !=
		        store.getReverseTransitive(1).end());
	}
}
