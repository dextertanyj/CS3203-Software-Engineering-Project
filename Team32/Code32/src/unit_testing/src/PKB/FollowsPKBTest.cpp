#include "MockUtilities.h"
#include "PKB/FollowsRelation.h"
#include "PKB/PKB.h"
#include "catch.hpp"
#include "catch_tools.h"

PKB generateFollowTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Read);
	return pkb;
}

TEST_CASE("PKB::FollowsRelation Internal Methods") {
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

TEST_CASE("PKB::PKB Facade Follows Methods") {
	PKB pkb = generateFollowTestPKB();
	StmtRef s_1 = 1;
	StmtRef s_2 = 2;
	StmtRef s_3 = 3;
	StmtRef s_4 = 4;
	StmtRef s_5 = 5;
	StmtRef s_6 = 6;
	StmtRef s_int_max = INT32_MAX;
	StmtRef s_minus_one = -1;

	SECTION("PKB::PKB::setFollows Check validity of arguments") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);
		// Invalid setting of negative statement number to statement map.
		CHECK_THROWS(pkb.setStmtType(-1, StmtType::Assign));

		// Verify that normal setting works.
		CHECK_NOTHROW(pkb.setFollows(s_1, s_2));
		CHECK_NOTHROW(pkb.setFollows(s_2, s_3));
		pkb = generateFollowTestPKB();

		// If s_1 already has a follower, it should not be able to have a new direct follower.
		CHECK_NOTHROW(pkb.setFollows(s_1, s_2));
		CHECK_THROWS(pkb.setFollows(s_1, s_int_max));
		pkb = generateFollowTestPKB();

		// If s_3 already follows s_2, then it should not be allowed to follow s_1.
		CHECK_NOTHROW(pkb.setFollows(s_2, s_3));
		CHECK_THROWS(pkb.setFollows(s_1, s_3));
		pkb = generateFollowTestPKB();

		// Verify that improper arguments lead to an exception thrown.
		CHECK_THROWS(pkb.setFollows(s_1, s_1));
		CHECK_THROWS(pkb.setFollows(s_minus_one, s_1));
		CHECK_THROWS(pkb.setFollows(s_2, s_1));
		CHECK_THROWS(pkb.setFollows(s_3, s_3));
		CHECK_THROWS(pkb.setFollows(s_minus_one, s_2));
		CHECK_THROWS(pkb.setFollows(s_3, s_minus_one));
	}

	SECTION("PKB::PKB::checkFollows") {
		pkb.setFollows(s_1, s_2);
		pkb.setFollows(s_2, s_3);

		// Ensure simple follow relation is stored successfully.
		CHECK(pkb.checkFollows(s_1, s_2));
		CHECK(pkb.checkFollows(s_2, s_3));

		// Ensure Follows* behavior does not appear for simple follow.
		CHECK_FALSE(pkb.checkFollows(s_1, s_3));

		// More negative cases.
		CHECK_FALSE(pkb.checkFollows(s_1, s_1));
		CHECK_FALSE(pkb.checkFollows(s_2, s_1));
		CHECK_FALSE(pkb.checkFollows(s_3, s_2));
	}

	SECTION("PKB::PKB::getFollower") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);

		pkb.setFollows(s_1, s_4);
		pkb.setFollows(s_2, s_3);
		pkb.setFollows(s_4, s_int_max);

		CHECK(pkb.getFollower(s_1)->reference == s_4);
		CHECK(pkb.getFollower(s_2)->reference == s_3);
		CHECK(pkb.getFollower(s_3) == nullptr);
		CHECK(pkb.getFollower(s_int_max) == nullptr);

		// Not stored into PKB.
		CHECK(pkb.getFollower(s_5) == nullptr);
		CHECK_THROWS_AS(pkb.getFollower(s_minus_one), std::invalid_argument);
	}

	SECTION("PKB::PKB::getPreceding") {
		pkb.setStmtType(INT32_MAX, StmtType::Read);

		pkb.setFollows(s_1, s_4);
		pkb.setFollows(s_2, s_3);
		pkb.setFollows(s_4, s_int_max);

		CHECK(pkb.getPreceding(s_1) == nullptr);
		CHECK(pkb.getPreceding(s_2) == nullptr);
		CHECK(pkb.getPreceding(s_3)->reference == s_2);
		CHECK(pkb.getPreceding(s_4)->reference == s_1);
		CHECK(pkb.getPreceding(s_int_max)->reference == s_4);

		// Not stored in PKB.
		CHECK(pkb.getPreceding(s_5) == nullptr);
		CHECK_THROWS_AS(pkb.getPreceding(s_minus_one), std::invalid_argument);
	}

	// Nesting levels as such:
	// 1-->2----------->6
	//       3-->4-->5
	SECTION("PKB::PKB::getFollowerStar") {
		pkb.setFollows(s_1, s_2);
		pkb.setFollows(s_2, s_6);
		pkb.setFollows(s_3, s_4);
		pkb.setFollows(s_4, s_5);
		pkb.populateComplexRelations();
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_1 = {stmt_info_map.at(s_2), stmt_info_map.at(s_6)};
		CHECK(pkb.getFollowerStar(s_1) == expected_follow_star_set_s_1);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_2 = {stmt_info_map.at(s_6)};
		CHECK(pkb.getFollowerStar(s_2) == expected_follow_star_set_s_2);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_3 = {stmt_info_map.at(s_4), stmt_info_map.at(s_5)};
		CHECK(pkb.getFollowerStar(s_3) == expected_follow_star_set_s_3);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_4 = {stmt_info_map.at(s_5)};
		CHECK(pkb.getFollowerStar(s_4) == expected_follow_star_set_s_4);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_5 = {};
		CHECK(pkb.getFollowerStar(s_5) == expected_follow_star_set_s_5);

		unordered_set<shared_ptr<StmtInfo>> expected_follow_star_set_s_int_max = {};
		CHECK(pkb.getFollowerStar(s_6) == expected_follow_star_set_s_int_max);

	}

	SECTION("PKB::PKB::getPrecedingStar") {
		pkb.setFollows(s_1, s_2);
		pkb.setFollows(s_2, s_6);
		pkb.setFollows(s_3, s_4);
		pkb.setFollows(s_4, s_5);
		pkb.populateComplexRelations();
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_info_map = pkb.getStmtInfoMap();

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_6 = {stmt_info_map.at(s_1), stmt_info_map.at(s_2)};
		CHECK(pkb.getPrecedingStar(s_6) == expected_preceding_star_set_s_6);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_5 = {stmt_info_map.at(s_3), stmt_info_map.at(s_4)};
		CHECK(pkb.getPrecedingStar(s_5) == expected_preceding_star_set_s_5);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_4 = {stmt_info_map.at(s_3)};
		CHECK(pkb.getPrecedingStar(s_4) == expected_preceding_star_set_s_4);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_3 = {};
		CHECK(pkb.getPrecedingStar(s_3) == expected_preceding_star_set_s_3);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_2 = {stmt_info_map.at(s_1)};
		CHECK(pkb.getPrecedingStar(s_2) == expected_preceding_star_set_s_2);

		unordered_set<shared_ptr<StmtInfo>> expected_preceding_star_set_s_1 = {};
		CHECK(pkb.getPrecedingStar(s_1) == expected_preceding_star_set_s_1);
	}
}
