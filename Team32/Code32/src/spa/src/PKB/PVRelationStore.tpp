#ifndef SPA_SRC_PKB_PVRELATIONSTORE_TPP
#define SPA_SRC_PKB_PVRELATIONSTORE_TPP

#include "PKB/PVRelationStore.h"

template <class T>
PKB::PVRelationStore<T>::PVRelationStore() = default;

template <class T>
void PKB::PVRelationStore<T>::set(const ProcRef& proc, const VarRef& variable) {
	assert(variable.length() != 0);
	assert(proc.length() != 0);

	auto variable_iter = variable_key_map.find(variable);
	auto proc_iter = proc_key_map.find(proc);

	if (proc_iter == proc_key_map.end()) {
		proc_key_map.insert({proc, {variable}});
	} else {
		proc_iter->second.insert(variable);
	}

	if (variable_iter == variable_key_map.end()) {
		variable_key_map.insert({variable, {proc}});
	} else {
		variable_iter->second.insert(proc);
	}
}

template <class T>
void PKB::PVRelationStore<T>::set(const ProcRef& proc, const VarRefSet& variables) {
	for (const VarRef& variable : variables) {
		assert(variable.length() != 0);
	}
	assert(proc.length() != 0);

	auto proc_iter = proc_key_map.find(proc);
	if (proc_iter == proc_key_map.end()) {
		proc_key_map.insert({proc, variables});
	} else {
		proc_iter->second.insert(variables.begin(), variables.end());
	}

	for (const VarRef& variable : variables) {
		auto variable_iter = variable_key_map.find(variable);
		if (variable_iter == variable_key_map.end()) {
			variable_key_map.insert({variable, {proc}});
		} else {
			variable_iter->second.insert(proc);
		}
	}
}

template <class T>
bool PKB::PVRelationStore<T>::check(const ProcRef& proc, const VarRef& variable) {
	assert(variable.length() != 0);
	assert(proc.length() != 0);

	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return false;
	}

	return any_of(iter->second.begin(), iter->second.end(), [proc](const ProcRef& procedure) { return procedure == proc; });
}

template <class T>
VarRefSet PKB::PVRelationStore<T>::getByProc(const ProcRef& proc) {
	assert(proc.length() != 0);

	auto iter = proc_key_map.find(proc);
	if (iter == proc_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
ProcRefSet PKB::PVRelationStore<T>::getByVar(const VarRef& variable) {
	assert(variable.length() != 0);

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
