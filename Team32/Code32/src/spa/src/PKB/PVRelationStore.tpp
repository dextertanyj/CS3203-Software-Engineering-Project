#ifndef SPA_SRC_PKB_PVRELATIONSTORE_TPP
#define SPA_SRC_PKB_PVRELATIONSTORE_TPP

#include "PKB/PVRelationStore.h"

#include <cassert>

template <class T>
PKB::PVRelationStore<T>::PVRelationStore() = default;

template <class T>
void PKB::PVRelationStore<T>::set(const ProcRef& proc, const VarRef& variable) {
	assert(variable.length() != 0);
	assert(proc.length() != 0);

	proc_key_map[proc].emplace(variable);
	variable_key_map[variable].emplace(proc);
}

template <class T>
void PKB::PVRelationStore<T>::set(const ProcRef& proc, const VarRefSet& variables) {
	assert(all_of(variables.begin(), variables.end(), [](const VarRef& variable) { return variable.length() != 0; }));
	assert(proc.length() != 0);

	proc_key_map[proc].insert(variables.begin(), variables.end());

	for (const VarRef& variable : variables) {
		variable_key_map[variable].emplace(proc);
	}
}

template <class T>
bool PKB::PVRelationStore<T>::check(const ProcRef& proc, const VarRef& variable) {
	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return false;
	}

	return any_of(iter->second.begin(), iter->second.end(), [proc](const ProcRef& procedure) { return procedure == proc; });
}

template <class T>
VarRefSet PKB::PVRelationStore<T>::getByProc(const ProcRef& proc) {
	auto iter = proc_key_map.find(proc);
	if (iter == proc_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
ProcRefSet PKB::PVRelationStore<T>::getByVar(const VarRef& variable) {
	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
void PKB::PVRelationStore<T>::clear() {
	variable_key_map.clear();
	proc_key_map.clear();
}

#endif
