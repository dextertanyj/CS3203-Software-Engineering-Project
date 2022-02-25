#include "VariableStore.h"

PKB::VariableStore::VariableStore() = default;

void PKB::VariableStore::insert(const VarRef& variable) { store.insert(variable); }

void PKB::VariableStore::insert(const VarRefSet& variables) { store.insert(variables.begin(), variables.end()); }

unordered_set<VarRef> PKB::VariableStore::getAll() { return store; }
