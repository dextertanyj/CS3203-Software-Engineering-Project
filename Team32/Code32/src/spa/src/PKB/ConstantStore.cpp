#include "ConstantStore.h"

ConstantStore::ConstantStore() = default;

void ConstantStore::insert(ConstVal value) { store.insert(value); }

void ConstantStore::insert(const unordered_set<ConstVal>& values) { store.insert(values.begin(), values.end()); }

unordered_set<ConstVal> ConstantStore::getAll() { return store; }
