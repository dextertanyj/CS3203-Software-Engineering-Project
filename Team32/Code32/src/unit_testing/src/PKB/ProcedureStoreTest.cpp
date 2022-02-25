#include "PKB/ProcedureStore.h"

#include "MockUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::ProcedureStore::setProc Success Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(3, StmtType::IfStmt);
	vector<shared_ptr<StmtInfo>> stmt_lst{s1, s2, s3};
	store.setProc("test", stmt_lst);
	vector<shared_ptr<StmtInfo>> res = store.getStmtsByProc("test");
	REQUIRE_EQUALS(res.size(), 3);
	REQUIRE(find(res.begin(), res.end(), s1) != res.end());
	REQUIRE(find(res.begin(), res.end(), s2) != res.end());
	REQUIRE(find(res.begin(), res.end(), s3) != res.end());
}

TEST_CASE("PKB::ProcedureStore::setProc Invalid StmtNo Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(0, StmtType::Assign);
	vector<shared_ptr<StmtInfo>> stmt_lst{s1};
	REQUIRE_THROWS_AS(store.setProc("test", vector<shared_ptr<StmtInfo>>{s1}), std::exception);
	vector<shared_ptr<StmtInfo>> res = store.getStmtsByProc("test");
	REQUIRE_EQUALS(res.size(), 0);
}

TEST_CASE("PKB::ProcedureStore::setProc Stmt Part of Another Procedure Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	vector<shared_ptr<StmtInfo>> stmt_lst{s1};
	store.setProc("test", stmt_lst);
	vector<shared_ptr<StmtInfo>> res = store.getStmtsByProc("test");
	REQUIRE_EQUALS(res.size(), 1);
	REQUIRE(find(res.begin(), res.end(), s1) != res.end());
	REQUIRE_THROWS_AS(store.setProc("test2", stmt_lst), std::exception);
}

TEST_CASE("PKB::ProcedureStore::setProc Duplicate Procedure Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	vector<shared_ptr<StmtInfo>> stmt_lst{s1};
	store.setProc("test", stmt_lst);
	vector<shared_ptr<StmtInfo>> res = store.getStmtsByProc("test");
	REQUIRE_EQUALS(res.size(), 1);
	REQUIRE(find(res.begin(), res.end(), s1) != res.end());
	REQUIRE_THROWS_AS(store.setProc("test", stmt_lst), std::exception);
}

TEST_CASE("PKB::ProcedureStore::setProc Duplicate Statement Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Print);
	vector<shared_ptr<StmtInfo>> stmt_lst{s1, s1};
	REQUIRE_THROWS_AS(store.setProc("test", stmt_lst), std::exception);
}

TEST_CASE("PKB::ProcedureStore::setCall Success Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(4, StmtType::Call);
	store.setCall(s1, "test");
	REQUIRE_EQUALS(store.getProcByCall(s1), "test");
}

TEST_CASE("PKB::ProcedureStore::setCall Non-Call Type Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(4, StmtType::Read);
	REQUIRE_THROWS_AS(store.setCall(s1, "test"), std::exception);
}

TEST_CASE("PKB::ProcedureStore::setCall Invalid StmtNo Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(0, StmtType::Call);
	REQUIRE_THROWS_AS(store.setCall(s1, "test"), std::exception);
}

TEST_CASE("PKB::ProcedureStore::setCall More Than 1 Call Stmt Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(4, StmtType::Call);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(5, StmtType::Call);
	store.setCall(s1, "test");
	REQUIRE_THROWS_AS(store.setCall(s2, "test"), std::exception);
}

TEST_CASE("PKB::ProcedureStore::getStmtsByProc Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(4, StmtType::Call);
	store.setProc("test", vector<shared_ptr<StmtInfo>>{s1});
	REQUIRE_EQUALS(store.getStmtsByProc("test").size(), 1);
	REQUIRE_EQUALS(store.getStmtsByProc("Test").size(), 0);
}

TEST_CASE("PKB::ProcedureStore::getProcByCall Invalid StmtNo Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(0, StmtType::Call);
	REQUIRE_THROWS_AS(store.getProcByCall(s1), std::exception);
}

TEST_CASE("PKB::ProcedureStore::getProcByCall Invalid Type Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::IfStmt);
	REQUIRE_THROWS_AS(store.getProcByCall(s1), std::exception);
}

TEST_CASE("PKB::ProcedureStore::getProcByStmt Success Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Call);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	store.setProc("test", vector<shared_ptr<StmtInfo>>{s1});
	REQUIRE_EQUALS(store.getProcByStmt(s1), "test");
	REQUIRE_EQUALS(store.getProcByStmt(s2), "");
}

TEST_CASE("PKB::ProcedureStore::getProcByStmt Invalid StmtNo Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(0, StmtType::Call);
	REQUIRE_THROWS_AS(store.getProcByStmt(s1), std::exception);
}

TEST_CASE("PKB::ProcedureStore::getProcListByStmtList Success Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Call);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> s3 = MockUtilities::createStmtInfo(2, StmtType::IfStmt);
	store.setProc("test", vector<shared_ptr<StmtInfo>>{s1});
	store.setProc("test2", vector<shared_ptr<StmtInfo>>{s2});
	unordered_set<ProcRef> res = store.getProcListByStmtList(unordered_set<shared_ptr<StmtInfo>>{s1, s2, s3});
	REQUIRE_EQUALS(res.size(), 2);
	REQUIRE(res.find("test") != res.end());
	REQUIRE(res.find("test2") != res.end());
}

TEST_CASE("PKB::ProcedureStore::getProcListByStmtList Invalid StmtNo Test") {
	PKB::ProcedureStore store = PKB::ProcedureStore();
	shared_ptr<StmtInfo> s1 = MockUtilities::createStmtInfo(1, StmtType::Call);
	shared_ptr<StmtInfo> s2 = MockUtilities::createStmtInfo(0, StmtType::Assign);
	REQUIRE_THROWS_AS(store.getProcListByStmtList(unordered_set<shared_ptr<StmtInfo>>{s1, s2}), std::exception);
}