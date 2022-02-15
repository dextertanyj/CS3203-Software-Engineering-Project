#include "PKB/ConstantStore.h"

#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::ConstantStore::insert Value Test") {
    ConstantStore store = ConstantStore();
    store.insert(0);
    store.insert(5);
    unordered_set<int> set = store.getAll();
    REQUIRE_EQUALS(set.size(), 2);
    REQUIRE(set.find(0) != set.end());
    REQUIRE(set.find(5) != set.end());
    REQUIRE_FALSE(set.find(10) != set.end());
}

TEST_CASE("PKB::ConstantStore::insert Set Test") {
    ConstantStore store = ConstantStore();
    unordered_set<int> constants = unordered_set<int>();
    store.insert(constants);
    unordered_set<int> set = store.getAll();
    REQUIRE_EQUALS(set.size(), 0);
    constants.insert(0);
    constants.insert(5);
    store.insert(constants);
    set = store.getAll();
    REQUIRE_EQUALS(set.size(), 2);
    REQUIRE(set.find(0) != set.end());
    REQUIRE(set.find(5) != set.end());
}
