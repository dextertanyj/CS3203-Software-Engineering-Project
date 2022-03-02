#include "PKB/InfoStore.tpp"

#include "TestUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::StatementStore::insert Success Test") {
	PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo> store = PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo>();
	store.insert(1, StmtType::IfStmt);
	store.insert(2, StmtType::Assign);
	store.insert(3, StmtType::Call);
	store.insert(4, StmtType::Read);
	store.insert(5, StmtType::Print);
	store.insert(6, StmtType::WhileStmt);
	REQUIRE_EQUALS(store.getAll().size(), 6);
	REQUIRE_EQUALS(store.get(1)->getType(), StmtType::IfStmt);
	REQUIRE_EQUALS(store.get(2)->getType(), StmtType::Assign);
	REQUIRE_EQUALS(store.get(3)->getType(), StmtType::Call);
	REQUIRE_EQUALS(store.get(4)->getType(), StmtType::Read);
	REQUIRE_EQUALS(store.get(5)->getType(), StmtType::Print);
	REQUIRE_EQUALS(store.get(6)->getType(), StmtType::WhileStmt);
	REQUIRE_EQUALS(store.getAll().size(), 6);
}

TEST_CASE("PKB::StatementStore::insert Duplicate StmtNo Test") {
	PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo> store = PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo>();
	store.insert(1, StmtType::IfStmt);
	REQUIRE_THROWS_AS(store.insert(1, StmtType::Assign), std::exception);
	REQUIRE_EQUALS(store.get(1)->getType(), StmtType::IfStmt);
}

TEST_CASE("PKB::StatementStore::insert Invalid StmtNo Test") {
	PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo> store = PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo>();
	REQUIRE_THROWS_AS(store.insert(0, StmtType::Assign), std::exception);
	REQUIRE_EQUALS(store.getAll().size(), 0);
}

TEST_CASE("PKB::StatementStore::get Success Test") {
	PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo> store = PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo>();
	store.insert(1, StmtType::IfStmt);
	store.insert(5, StmtType::Assign);
	REQUIRE_EQUALS(store.get(1)->getType(), StmtType::IfStmt);
	REQUIRE_EQUALS(store.get(5)->getType(), StmtType::Assign);
	REQUIRE_EQUALS(store.get(4), nullptr);
}

TEST_CASE("PKB::StatementStore::get Invalid StmtNo Test") {
	PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo> store = PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo>();
	REQUIRE_THROWS_AS(store.get(0), std::exception);
}

TEST_CASE("PKB::StatementStore::getAll Test") {
	PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo> store = PKB::InfoStore<StmtRef, StmtType, PKB::StatementInfo>();
	REQUIRE_EQUALS(store.getAll().size(), 0);
	store.insert(1, StmtType::IfStmt);
	store.insert(5, StmtType::Assign);
	REQUIRE_EQUALS(store.getAll().size(), 2);
}
