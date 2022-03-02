#include "PKB/StatementStore.h"

#include "TestUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::StatementStore::insert Success Test") {
	PKB::StatementStore store = PKB::StatementStore();
	store.insert(1, StmtType::IfStmt);
	store.insert(2, StmtType::Assign);
	store.insert(3, StmtType::Call);
	store.insert(4, StmtType::Read);
	store.insert(5, StmtType::Print);
	store.insert(6, StmtType::WhileStmt);
	REQUIRE_EQUALS(store.getAll().size(), 6);
	REQUIRE_EQUALS(store.get(1)->type, StmtType::IfStmt);
	REQUIRE_EQUALS(store.get(2)->type, StmtType::Assign);
	REQUIRE_EQUALS(store.get(3)->type, StmtType::Call);
	REQUIRE_EQUALS(store.get(4)->type, StmtType::Read);
	REQUIRE_EQUALS(store.get(5)->type, StmtType::Print);
	REQUIRE_EQUALS(store.get(6)->type, StmtType::WhileStmt);
	REQUIRE_EQUALS(store.getAll().size(), 6);
}

TEST_CASE("PKB::StatementStore::insert Duplicate StmtNo Test") {
	PKB::StatementStore store = PKB::StatementStore();
	store.insert(1, StmtType::IfStmt);
	REQUIRE_THROWS_AS(store.insert(1, StmtType::Assign), std::exception);
	REQUIRE_EQUALS(store.get(1)->type, StmtType::IfStmt);
}

TEST_CASE("PKB::StatementStore::insert Invalid StmtNo Test") {
	PKB::StatementStore store = PKB::StatementStore();
	REQUIRE_THROWS_AS(store.insert(0, StmtType::Assign), std::exception);
	REQUIRE_EQUALS(store.getAll().size(), 0);
}

TEST_CASE("PKB::StatementStore::get Success Test") {
	PKB::StatementStore store = PKB::StatementStore();
	store.insert(1, StmtType::IfStmt);
	store.insert(5, StmtType::Assign);
	REQUIRE_EQUALS(store.get(1)->type, StmtType::IfStmt);
	REQUIRE_EQUALS(store.get(5)->type, StmtType::Assign);
	REQUIRE_EQUALS(store.get(4), nullptr);
}

TEST_CASE("PKB::StatementStore::get Invalid StmtNo Test") {
	PKB::StatementStore store = PKB::StatementStore();
	REQUIRE_THROWS_AS(store.get(0), std::exception);
}

TEST_CASE("PKB::StatementStore::getAll Test") {
	PKB::StatementStore store = PKB::StatementStore();
	REQUIRE_EQUALS(store.getAll().size(), 0);
	store.insert(1, StmtType::IfStmt);
	store.insert(5, StmtType::Assign);
	REQUIRE_EQUALS(store.getAll().size(), 2);
}
