#include "ConstantStore.h"

PKB::ConstantStore::ConstantStore() = default;

void PKB::ConstantStore::insert(ConstVal value) { store.insert(value); }

void PKB::ConstantStore::insert(const unordered_set<ConstVal>& values) { store.insert(values.begin(), values.end()); }

unordered_set<ConstVal> PKB::ConstantStore::getAll() { return store; }
