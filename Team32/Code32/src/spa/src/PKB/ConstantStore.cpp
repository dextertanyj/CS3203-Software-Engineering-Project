#include "ConstantStore.h"

ConstantStore::ConstantStore() = default;

void ConstantStore::insert(int value) { store.insert(value); }

void ConstantStore::insert(const unordered_set<int>& values) { store.insert(values.begin(), values.end()); }

unordered_set<int> ConstantStore::getAll() { return store; }
