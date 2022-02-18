#include <algorithm>

#include "PKB/Modifies.h"
#include "PKB/SVRelationStore.h"
#include "PKB/Uses.h"

template <class T>
SVRelationStore<T>::SVRelationStore() = default;

template <class T>
void SVRelationStore<T>::set(shared_ptr<StmtInfo> statement, VarRef variable) {
	StmtRef index = statement->reference;
	if (variable.length() == 0) {
		throw invalid_argument("Variable name must have length more than 0.");
	}

	if (!T::validate(this, statement, variable)) {
		throw "Relationship set error";
	}

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
void SVRelationStore<T>::set(shared_ptr<StmtInfo> statement, VarRefSet variables) {
	StmtRef index = statement->reference;
	for (const VarRef& variable : variables) {
		if (variable.length() == 0) {
			throw invalid_argument("Variable name must have length more than 0.");
		}
	}

	if (!T::validate(this, statement, variables)) {
		throw "Relationship set error";
	}

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
bool SVRelationStore<T>::check(StmtRef index, VarRef variable) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	if (variable.length() == 0) {
		throw invalid_argument("Variable name must have length more than 0.");
	}

	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return false;
	}

	return any_of(iter->second.begin(), iter->second.end(),
	              [index](const shared_ptr<StmtInfo>& statement) { return statement->reference == index; });
}

template <class T>
unordered_set<VarRef> SVRelationStore<T>::getByStmt(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto iter = statement_key_map.find(index);
	if (iter == statement_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
unordered_set<shared_ptr<StmtInfo>> SVRelationStore<T>::getByVar(VarRef variable) {
	if (variable.length() == 0) {
		throw invalid_argument("Variable name must have length more than 0.");
	}

	auto iter = variable_key_map.find(variable);
	if (iter == variable_key_map.end()) {
		return {};
	}
	return iter->second;
}

template <class T>
void SVRelationStore<T>::clear() {
	variable_key_map.clear();
	statement_key_map.clear();
}