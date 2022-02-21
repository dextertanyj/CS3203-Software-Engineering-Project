#include "PKB/VariableStore.h"

#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::VariableStore::insert Value Test") {
	PKB::VariableStore store = PKB::VariableStore();
    store.insert("x");
    store.insert("y");
    unordered_set<VarRef> set = store.getAll();
    REQUIRE_EQUALS(set.size(), 2);
    REQUIRE(set.find("x") != set.end());
    REQUIRE(set.find("y") != set.end());
    REQUIRE_FALSE(set.find("z") != set.end());
}

TEST_CASE("PKB::VariableStore::insert Set Test") {
	PKB::VariableStore store = PKB::VariableStore();
    unordered_set<VarRef> constants = unordered_set<VarRef>();
    store.insert(constants);
    unordered_set<VarRef> set = store.getAll();
    REQUIRE_EQUALS(set.size(), 0);
    constants.insert("x");
    constants.insert("y");
    store.insert(constants);
    set = store.getAll();
    REQUIRE_EQUALS(set.size(), 2);
    REQUIRE(set.find("x") != set.end());
    REQUIRE(set.find("y") != set.end());
    store.insert(constants);
    REQUIRE_EQUALS(set.size(), 2);
}
