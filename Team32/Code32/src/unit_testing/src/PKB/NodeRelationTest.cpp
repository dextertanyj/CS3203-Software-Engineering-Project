#include "PKB/CFG/NodeInfo.h"
#include "PKB/PKB.h"
#include "TestUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::NodeRelation::insertForward Test") {
	// No more than 3 next relations per if node
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(1, StmtType::IfStmt));
	REQUIRE_NOTHROW(relation.insertForward(TestUtilities::createNodeInfo(2, StmtType::Read)));
	REQUIRE_NOTHROW(relation.insertForward(TestUtilities::createNodeInfo(3, StmtType::Print)));
	REQUIRE_NOTHROW(relation.insertForward(TestUtilities::createNodeInfo(4, StmtType::Assign)));
	REQUIRE_THROWS_AS(relation.insertForward(TestUtilities::createNodeInfo(7, StmtType::Assign)), logic_error);
	REQUIRE_THROWS_AS(relation.insertForward(TestUtilities::createNodeInfo(1, StmtType::Read)), logic_error);
}

TEST_CASE("PKB::NodeRelation::insertReverse Test") {
	// No more than 3 previous relations per if node
	PKB::NodeRelation if_relation = PKB::NodeRelation(TestUtilities::createNodeInfo(7, StmtType::IfStmt));
	REQUIRE_NOTHROW(if_relation.insertReverse(TestUtilities::createNodeInfo(6, StmtType::Read)));
	REQUIRE_NOTHROW(if_relation.insertReverse(TestUtilities::createNodeInfo(5, StmtType::Print)));
	REQUIRE_NOTHROW(if_relation.insertReverse(TestUtilities::createNodeInfo(4, StmtType::Assign)));
	REQUIRE_THROWS_AS(if_relation.insertReverse(TestUtilities::createNodeInfo(3, StmtType::Assign)), logic_error);

	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(5, StmtType::Read));
	REQUIRE_NOTHROW(relation.insertReverse(TestUtilities::createNodeInfo(8, StmtType::Read)));
	REQUIRE_THROWS_AS(relation.insertReverse(TestUtilities::createNodeInfo(9, StmtType::Assign)), logic_error);
}

TEST_CASE("PKB::NodeRelation::getSelf() Test") {
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(7, StmtType::Read));
	REQUIRE_EQUALS(relation.getSelf().get()->getIdentifier(), 7);
	REQUIRE_EQUALS(relation.getSelf().get()->getType(), StmtType::Read);
}

TEST_CASE("PKB::NodeRelation::appendReverseTransitive Test") {
	shared_ptr<PKB::NodeInfo> s_1 = TestUtilities::createNodeInfo(5, StmtType::Print);
	shared_ptr<PKB::NodeInfo> s_2 = TestUtilities::createNodeInfo(6, StmtType::Assign);
	shared_ptr<PKB::NodeInfo> s_3 = TestUtilities::createNodeInfo(7, StmtType::IfStmt);
	shared_ptr<PKB::NodeInfo> s_4 = TestUtilities::createNodeInfo(1, StmtType::Print);
	unordered_set<shared_ptr<PKB::NodeInfo>> set({s_1, s_2, s_3});
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(4, StmtType::Read));
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set));

	// TODO: recursive call detected???
}

TEST_CASE("PKB::NodeRelation::appendForwardTransitive Test") {
	shared_ptr<PKB::NodeInfo> s_1 = TestUtilities::createNodeInfo(5, StmtType::Print);
	shared_ptr<PKB::NodeInfo> s_2 = TestUtilities::createNodeInfo(6, StmtType::Assign);
	shared_ptr<PKB::NodeInfo> s_3 = TestUtilities::createNodeInfo(7, StmtType::IfStmt);
	shared_ptr<PKB::NodeInfo> s_4 = TestUtilities::createNodeInfo(1, StmtType::Print);
	unordered_set<shared_ptr<PKB::NodeInfo>> set({s_1, s_2, s_3});
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(4, StmtType::Read));
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set));

	// TODO: recursive call detected???
}

TEST_CASE("PKB::NodeRelation::getForward Test") {
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(3, StmtType::Read));
	REQUIRE_EQUALS(relation.getForward(), unordered_set<shared_ptr<PKB::NodeInfo>>());
	shared_ptr<PKB::NodeInfo> node = TestUtilities::createNodeInfo(2, StmtType::Print);
	REQUIRE_NOTHROW(relation.insertForward(node));
	REQUIRE_EQUALS(relation.getForward().size(), 1);
	REQUIRE(relation.getForward().find(node) != relation.getForward().end());
}

TEST_CASE("PKB::NodeRelation::getReverse Test") {
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(3, StmtType::Read));
	REQUIRE_EQUALS(relation.getReverse(), unordered_set<shared_ptr<PKB::NodeInfo>>());
	shared_ptr<PKB::NodeInfo> node = TestUtilities::createNodeInfo(2, StmtType::Print);
	REQUIRE_NOTHROW(relation.insertReverse(node));
	REQUIRE_EQUALS(relation.getReverse().size(), 1);
	REQUIRE(relation.getReverse().find(node) != relation.getReverse().end());
}

TEST_CASE("PKB::NodeRelation::getForwardTransitive Test") {
	shared_ptr<PKB::NodeInfo> s_1 = TestUtilities::createNodeInfo(1, StmtType::Print);
	shared_ptr<PKB::NodeInfo> s_2 = TestUtilities::createNodeInfo(2, StmtType::Assign);
	shared_ptr<PKB::NodeInfo> s_3 = TestUtilities::createNodeInfo(3, StmtType::IfStmt);
	shared_ptr<PKB::NodeInfo> s_4 = TestUtilities::createNodeInfo(7, StmtType::Print);
	unordered_set<shared_ptr<PKB::NodeInfo>> set({s_1, s_2, s_3});
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(6, StmtType::Read));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
	unordered_set<shared_ptr<PKB::NodeInfo>> set2;
	REQUIRE_NOTHROW(relation.appendForwardTransitive(set2));
	REQUIRE_EQUALS(relation.getForwardTransitive().size(), 3);
}

TEST_CASE("PKB::NodeRelation::getReverseTransitive Test") {
	shared_ptr<PKB::NodeInfo> s_1 = TestUtilities::createNodeInfo(1, StmtType::Print);
	shared_ptr<PKB::NodeInfo> s_2 = TestUtilities::createNodeInfo(2, StmtType::Assign);
	shared_ptr<PKB::NodeInfo> s_3 = TestUtilities::createNodeInfo(3, StmtType::IfStmt);
	shared_ptr<PKB::NodeInfo> s_4 = TestUtilities::createNodeInfo(7, StmtType::Print);
	unordered_set<shared_ptr<PKB::NodeInfo>> set({s_1, s_2, s_3});
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(6, StmtType::Read));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 0);
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
	unordered_set<shared_ptr<PKB::NodeInfo>> set2;
	REQUIRE_NOTHROW(relation.appendReverseTransitive(set2));
	REQUIRE_EQUALS(relation.getReverseTransitive().size(), 3);
}

TEST_CASE("PKB::NodeRelation::setCFGIndex Test") {
	PKB::NodeRelation relation = PKB::NodeRelation(TestUtilities::createNodeInfo(6, StmtType::Read));
	relation.setCFGIndex(2);
	REQUIRE_EQUALS(relation.getSelf().get()->getUniqueIndex(), 2);
	relation.setCFGIndex(3);
	REQUIRE_EQUALS(relation.getSelf().get()->getUniqueIndex(), 3);
}

// TODO: test for optimize
