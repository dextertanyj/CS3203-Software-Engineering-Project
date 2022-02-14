#include "VariableStore.h"

VariableStore::VariableStore() = default;

void VariableStore::insert(const VarRef& variable) { store.insert(variable); }

void VariableStore::insert(const VarRefSet& variables) { store.insert(variables.begin(), variables.end()); }

unordered_set<VarRef> VariableStore::getAll() { return store; }
