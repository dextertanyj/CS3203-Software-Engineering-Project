#ifndef SPA_SRC_PKB_SVRELATIONSTORE_TPP
#define SPA_SRC_PKB_SVRELATIONSTORE_TPP

#include "PKB/SVRelationStore.h"

#include <algorithm>
#include <stdexcept>

#include "PKB/ModifiesSRelation.h"
#include "PKB/UsesSRelation.h"

template <class T>
PKB::SVRelationStore<T>::SVRelationStore() = default;

template <class T>
void PKB::SVRelationStore<T>::set(shared_ptr<StmtInfo> statement, VarRef variable) {
	StmtRef index = statement->getIdentifier();

	assert(variable.length() != 0);
	assert(T::validate(this, statement, variable));

	auto variable_iter = variable_key_map.find(variable);
	auto statement_iter = statement_key_map.find(index);

	if (statement_iter == statement_key_map.end()) {
		statement_key_map.insert({index, {variable}});
	} else {
		statement_iter->second.insert(variable);
	}

	if (variable_iter == variable_key_map.end()) {
		variable_key_map.insert({variable, {statement}});
	} else {
		variable_iter->second.insert(statement);
	}
}

template <class T>
void PKB::SVRelationStore<T>::set(shared_ptr<StmtInfo> statement, VarRefSet variables) {
	StmtRef index = statement->getIdentifier();
	assert(all_of(variables.begin(), variables.end(), [](const VarRef& variable) { return variable.length() != 0; }));
	assert(T::validate(this, statement, variables));

	auto statement_iter = statement_key_map.find(index);
	if (statement_iter == statement_key_map.end()) {
		statement_key_map.insert({index, variables});
	} else {
		statement_iter->second.insert(variables.begin(), variables.end());
	}

	for (const VarRef& variable : variables) {
		auto variable_iter = variable_key_map.find(variable);
		if (variable_iter == variable_key_map.end()) {
			variable_key_map.insert({variable, {statement}});
		} else {
			variable_iter->second.insert(statement);
		}
	}
}

template <class T>
bool PKB::SVRelationStore<T>::check(StmtRef index, const VarRef& variable) {
	assert(index > 0);
	assert(variable.length() != 0);

	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return false;
	}

	return any_of(iter->second.begin(), iter->second.end(),
	              [index](const shared_ptr<StmtInfo>& statement) { return statement->getIdentifier() == index; });
}

template <class T>
VarRefSet PKB::SVRelationStore<T>::getByStmt(StmtRef index) {
	assert(index > 0);

	auto iter = statement_key_map.find(index);
	if (iter == statement_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
StmtInfoPtrSet PKB::SVRelationStore<T>::getByVar(const VarRef& variable) {
	assert(variable.length() != 0);

	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
void PKB::SVRelationStore<T>::clear() {
	variable_key_map.clear();
	statement_key_map.clear();
}

#endif  // SPA_SRC_PKB_SVRELATIONSTORE_TPP