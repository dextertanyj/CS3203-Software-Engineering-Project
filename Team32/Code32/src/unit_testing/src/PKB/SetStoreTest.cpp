#include "PKB/SetStore.tpp"

#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::SetStore::insert Item Test") {
	PKB::SetStore<string> store = PKB::SetStore<string>();

	SECTION("Standard") {
		store.insert("x");
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 1);
		REQUIRE(set.find("x") != set.end());
	}

	SECTION("Multiple Items") {
		store.insert("x");
		store.insert("y");
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 2);
		REQUIRE(set.find("x") != set.end());
		REQUIRE(set.find("y") != set.end());
	}

	SECTION("Duplicate Items") {
		store.insert("x");
		store.insert("x");
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 1);
		REQUIRE(set.find("x") != set.end());
	}
}

TEST_CASE("PKB::SetStore::insert Item Set Test") {
	PKB::SetStore<string> store = PKB::SetStore<string>();
	unordered_set<string> items = unordered_set<string>();

	SECTION("Empty Set") {
		store.insert(items);
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 0);
	}

	SECTION("Single Item") {
		items.insert("x");
		store.insert(items);
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 1);
		REQUIRE(set.find("x") != set.end());
	}

	SECTION("Multiple Items") {
		items.insert("x");
		items.insert("y");
		store.insert(items);
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 2);
		REQUIRE(set.find("x") != set.end());
		REQUIRE(set.find("y") != set.end());
	}

	SECTION("Duplicate Insert") {
		items.insert("x");
		items.insert("y");
		store.insert(items);
		items = unordered_set<string>({"x", "z"});
		store.insert(items);
		unordered_set<string> set = store.getAll();
		REQUIRE_EQUALS(set.size(), 3);
		REQUIRE(set.find("x") != set.end());
		REQUIRE(set.find("y") != set.end());
		REQUIRE(set.find("z") != set.end());
	}
}
