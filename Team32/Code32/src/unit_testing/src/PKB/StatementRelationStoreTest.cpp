#include "PKB/StatementRelationStore.h"

#include "MockUtilities.h"
#include "PKB/PKB.h"
#include "catch.hpp"

TEST_CASE("PKB::StatementRelationStore::set Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(4, StmtType::Read);

	SECTION("PKB::StatementRelationStore::set Follows Test") {
		StatementRelationStore follows_store = StatementRelationStore<FollowsRelation>();
		// Second statement must come after the first statement.
		REQUIRE_THROWS_AS(follows_store.set(s_2, s_1), invalid_argument);
		REQUIRE_THROWS_AS(follows_store.set(s_3, s_3), invalid_argument);

		CHECK_NOTHROW(follows_store.set(s_1, s_2));
		// A statement cannot have more than 1 follower.
		REQUIRE_THROWS_AS(follows_store.set(s_1, s_2), invalid_argument);

		CHECK_NOTHROW(follows_store.set(s_3, s_4));
		// A follower cannot follow more than 1 statement.
		REQUIRE_THROWS_AS(follows_store.set(s_2, s_4), invalid_argument);

		CHECK_NOTHROW(follows_store.set(s_2, s_3));
		follows_store.clear();
	}

	SECTION("PKB::StatementRelationStore::set Parents Test") {
		StatementRelationStore parents_store = StatementRelationStore<ParentRelation>();
		// Second statement must come after the first statement.
		REQUIRE_THROWS_AS(parents_store.set(s_2, s_1), invalid_argument);
		REQUIRE_THROWS_AS(parents_store.set(s_3, s_3), invalid_argument);

		CHECK_NOTHROW(parents_store.set(s_1, s_3));
		// A statement cannot have more than 1 parent.
		REQUIRE_THROWS_AS(parents_store.set(s_2, s_3), invalid_argument);

		parents_store.clear();

		CHECK_NOTHROW(parents_store.set(s_1, s_2));
		// A statement can have more than 1 child.
		CHECK_NOTHROW(parents_store.set(s_1, s_3));
		CHECK_NOTHROW(parents_store.set(s_1, s_4));
		parents_store.clear();
	}
}

TEST_CASE("PKB::StatementRelationStore::getForward Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(4, StmtType::Read);

	SECTION("PKB::StatementRelationStore::getForward Follows Test") {
		StatementRelationStore follows_store = StatementRelationStore<FollowsRelation>();
		follows_store.set(s_1, s_2);
		follows_store.set(s_2, s_3);
		follows_store.set(s_3, s_4);

		// StmtRef index less than or equal to 0 throws an error
		REQUIRE_THROWS_AS(follows_store.getForward(0), invalid_argument);

		CHECK(follows_store.getForward(1).empty());
		CHECK(follows_store.getForward(2) == unordered_set<shared_ptr<StmtInfo>>{s_1});
		CHECK(follows_store.getForward(3) == unordered_set<shared_ptr<StmtInfo>>{s_2});
		CHECK(follows_store.getForward(4) == unordered_set<shared_ptr<StmtInfo>>{s_3});

		// Check negative cases.
		CHECK(follows_store.getForward(5).empty());
		// Check transitive relation was not created.
		CHECK_FALSE(follows_store.getForward(4).size() > 1);
		follows_store.clear();
	}

	SECTION("PKB::StatementRelationStore::getForward Parents Test") {
		StatementRelationStore parents_store = StatementRelationStore<ParentRelation>();
		parents_store.set(s_1, s_2);
		parents_store.set(s_1, s_3);
		parents_store.set(s_3, s_4);

		// StmtRef index less than or equal to 0 throws an error
		REQUIRE_THROWS_AS(parents_store.getForward(0), invalid_argument);

		CHECK(parents_store.getForward(1).empty());
		CHECK(parents_store.getForward(2) == unordered_set<shared_ptr<StmtInfo>>{s_1});
		CHECK(parents_store.getForward(3) == unordered_set<shared_ptr<StmtInfo>>{s_1});
		CHECK(parents_store.getForward(4) == unordered_set<shared_ptr<StmtInfo>>{s_3});

		// Check negative cases.
		CHECK(parents_store.getForward(5).empty());
		// Check transitive relation was not created.
		CHECK_FALSE(parents_store.getForward(4).size() > 1);
	}
}

TEST_CASE("PKB::StatementRelationStore::getReverse Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(4, StmtType::Read);

	SECTION("PKB::StatementRelationStore::getReverse Follows Test") {
		StatementRelationStore follows_store = StatementRelationStore<FollowsRelation>();
		follows_store.set(s_1, s_2);
		follows_store.set(s_2, s_3);
		follows_store.set(s_3, s_4);

		// StmtRef index less than or equal to 0 throws an error
		REQUIRE_THROWS_AS(follows_store.getReverse(0), invalid_argument);

		CHECK(follows_store.getReverse(4).empty());
		CHECK(follows_store.getReverse(3) == unordered_set<shared_ptr<StmtInfo>>{s_4});
		CHECK(follows_store.getReverse(2) == unordered_set<shared_ptr<StmtInfo>>{s_3});
		CHECK(follows_store.getReverse(1) == unordered_set<shared_ptr<StmtInfo>>{s_2});

		// Check negative cases.
		CHECK(follows_store.getReverse(5).empty());
		// Check transitive relation was not created.
		CHECK_FALSE(follows_store.getReverse(1).size() > 1);
		follows_store.clear();
	}

	SECTION("PKB::StatementRelationStore::getReverse Parents Test") {
		StatementRelationStore parents_store = StatementRelationStore<ParentRelation>();
		parents_store.set(s_1, s_2);
		parents_store.set(s_1, s_3);
		parents_store.set(s_3, s_4);

		// StmtRef index less than or equal to 0 throws an error
		REQUIRE_THROWS_AS(parents_store.getReverse(0), invalid_argument);

		CHECK(parents_store.getReverse(1) == unordered_set<shared_ptr<StmtInfo>>{s_2, s_3});
		CHECK(parents_store.getReverse(2).empty());
		CHECK(parents_store.getReverse(3) == unordered_set<shared_ptr<StmtInfo>>{s_4});
		CHECK(parents_store.getReverse(4).empty());

		// Check negative cases.
		CHECK(parents_store.getReverse(5).empty());
		// Check transitive relation was not created.
		CHECK_FALSE(parents_store.getReverse(1).size() == 3);
	}
}
TEST_CASE("PKB::StatementRelationStore::getForwardTransitive Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(4, StmtType::Read);

	SECTION("PKB::StatementRelationStore::getForwardTransitive Follows Test") {
		StatementRelationStore follows_store = StatementRelationStore<FollowsRelation>();

		follows_store.set(s_1, s_2);
		follows_store.set(s_2, s_3);
		follows_store.set(s_3, s_4);
		FollowsRelation::optimize(follows_store);

		// StmtRef index less than or equal to 0
		REQUIRE_THROWS_AS(follows_store.getForwardTransitive(0), invalid_argument);

		CHECK(follows_store.getForwardTransitive(4) == unordered_set<shared_ptr<StmtInfo>>{s_1, s_2, s_3});
		CHECK(follows_store.getForwardTransitive(3) == unordered_set<shared_ptr<StmtInfo>>{s_1, s_2});
		CHECK(follows_store.getForwardTransitive(2) == unordered_set<shared_ptr<StmtInfo>>{s_1});
		CHECK(follows_store.getForwardTransitive(1).empty());
	}

	SECTION("PKB::StatementRelationStore::getForwardTransitive Parents Test") {
		StatementRelationStore parents_store = StatementRelationStore<ParentRelation>();

		parents_store.set(s_1, s_2);
		parents_store.set(s_1, s_3);
		parents_store.set(s_3, s_4);
		ParentRelation::optimize(parents_store);

		// StmtRef index less than or equal to 0
		REQUIRE_THROWS_AS(parents_store.getForwardTransitive(0), invalid_argument);

		CHECK(parents_store.getForwardTransitive(4) == unordered_set<shared_ptr<StmtInfo>>{s_1, s_3});
		CHECK(parents_store.getForwardTransitive(3) == unordered_set<shared_ptr<StmtInfo>>{s_1});
		CHECK(parents_store.getForwardTransitive(2) == unordered_set<shared_ptr<StmtInfo>>{s_1});
		CHECK(parents_store.getForwardTransitive(1).empty());
	}
}

TEST_CASE("PKB::StatementRelationStore::getReverseTransitive Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(4, StmtType::Read);

	SECTION("PKB::StatementRelationStore::getReverseTransitive Follows Test") {
		StatementRelationStore follows_store = StatementRelationStore<FollowsRelation>();

		follows_store.set(s_1, s_2);
		follows_store.set(s_2, s_3);
		follows_store.set(s_3, s_4);
		FollowsRelation::optimize(follows_store);

		// StmtRef index less than or equal to 0
		REQUIRE_THROWS_AS(follows_store.getReverseTransitive(0), invalid_argument);

		CHECK(follows_store.getReverseTransitive(1) == unordered_set<shared_ptr<StmtInfo>>{s_2, s_3, s_4});
		CHECK(follows_store.getReverseTransitive(2) == unordered_set<shared_ptr<StmtInfo>>{s_3, s_4});
		CHECK(follows_store.getReverseTransitive(3) == unordered_set<shared_ptr<StmtInfo>>{s_4});
		CHECK(follows_store.getReverseTransitive(4).empty());
	}

	SECTION("PKB::StatementRelationStore::getForwardTransitive Parents Test") {
		StatementRelationStore parents_store = StatementRelationStore<ParentRelation>();

		parents_store.set(s_1, s_2);
		parents_store.set(s_1, s_3);
		parents_store.set(s_3, s_4);
		ParentRelation::optimize(parents_store);

		// StmtRef index less than or equal to 0
		REQUIRE_THROWS_AS(parents_store.getReverseTransitive(0), invalid_argument);

		CHECK(parents_store.getReverseTransitive(1) == unordered_set<shared_ptr<StmtInfo>>{s_2, s_3, s_4});
		CHECK(parents_store.getReverseTransitive(2).empty());
		CHECK(parents_store.getReverseTransitive(3) == unordered_set<shared_ptr<StmtInfo>>{s_4});
		CHECK(parents_store.getReverseTransitive(4).empty());
	}
}

TEST_CASE("PKB::StatementRelationStore::isRelated Test") {
	shared_ptr<StmtInfo> s_1 = MockUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> s_2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	shared_ptr<StmtInfo> s_4 = MockUtilities::createStmtInfo(4, StmtType::Read);

	SECTION("PKB::StatementRelationStore::isRelated Follows Test") {
		StatementRelationStore follows_store = StatementRelationStore<FollowsRelation>();

		follows_store.set(s_1, s_2);
		follows_store.set(s_2, s_3);
		follows_store.set(s_3, s_4);
		FollowsRelation::optimize(follows_store);

		// StmtRef index must be less than or equal to 0. Front statement number must be < back statement number.
		REQUIRE_THROWS_AS(follows_store.isRelated(1, 0), invalid_argument);

		CHECK(follows_store.isRelated(1, 2));
		CHECK(follows_store.isRelated(2, 3));
		CHECK(follows_store.isRelated(3, 4));

		// Transitive relations should not be captured by isRelated. Only direct relations.
		CHECK_FALSE(follows_store.isRelated(1, 3));
		CHECK_FALSE(follows_store.isRelated(1, 4));
		CHECK_FALSE(follows_store.isRelated(2, 4));

		CHECK_FALSE(follows_store.isRelated(3, 2));
	}

	SECTION("PKB::StatementRelationStore::isRelated Parents Test") {
		StatementRelationStore parents_store = StatementRelationStore<ParentRelation>();

		parents_store.set(s_1, s_2);
		parents_store.set(s_1, s_3);
		parents_store.set(s_3, s_4);
		ParentRelation::optimize(parents_store);

		// StmtRef index must be less than or equal to 0. Front statement number must be < back statement number.
		REQUIRE_THROWS_AS(parents_store.isRelated(1, 0), invalid_argument);

		CHECK(parents_store.isRelated(1, 2));
		CHECK(parents_store.isRelated(1, 3));
		CHECK(parents_store.isRelated(3, 4));

		// Negative case.
		CHECK_FALSE(parents_store.isRelated(3, 2));
		CHECK_FALSE(parents_store.isRelated(2, 4));

		// Transitive relations should not be captured by isRelated. Only direct relations.
		CHECK_FALSE(parents_store.isRelated(1, 4));
	}
}

