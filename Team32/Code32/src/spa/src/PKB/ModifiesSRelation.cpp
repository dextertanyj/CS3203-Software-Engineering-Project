#include "PKB/ModifiesSRelation.h"

#include <algorithm>

#include "PKB/SVRelationStore.tpp"

bool PKB::ModifiesSRelation::validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement,
                                     const VarRef& variable) {
	StmtRef idx = statement->getIdentifier();
	if (statement->getType() == StmtType::Print) {
		throw invalid_argument("Print statements cannot modify a variable");
	}
	if (statement->getType() == StmtType::WhileStmt || statement->getType() == StmtType::IfStmt || statement->getType() == StmtType::Call) {
		return true;
	}
	auto statement_iter = store->statement_key_map.find(idx);
	// If the statement reference is not found in the SVRelationStore, then setModifies is valid.
	if (statement_iter == store->statement_key_map.end()) {
		return true;
	}
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(),
	                [variable](const VarRef& existing_var) { return existing_var != variable; }));
}

bool PKB::ModifiesSRelation::validate(SVRelationStore<ModifiesSRelation>* store, const shared_ptr<StmtInfo>& statement,
                                     const VarRefSet& variables) {
	StmtRef idx = statement->getIdentifier();
	if (statement->getType() == StmtType::Print) {
		throw invalid_argument("Print statements cannot modify a variable");
	}
	if (statement->getType() == StmtType::WhileStmt || statement->getType() == StmtType::IfStmt || statement->getType() == StmtType::Call) {
		return true;
	}
	if (variables.size() > 1) {
		return false;
	}
	VarRef variable = *variables.begin();
	auto statement_iter = store->statement_key_map.find(idx);
	if (statement_iter == store->statement_key_map.end()) {
		return true;
	}
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(),
	                [variable](const VarRef& existing_var) { return existing_var == variable; }));
}

void PKB::ModifiesSRelation::optimize(Types::StatementStore& statement_store, StatementRelationStore<ParentRelation>& parent_store,
                                     SVRelationStore<ModifiesSRelation>& store) {
	for (const auto& statement : statement_store.getAll()) {
		if (statement->getType() == StmtType::IfStmt || statement->getType() == StmtType::WhileStmt) {
			auto children = parent_store.getReverseTransitive(statement->getIdentifier());
			unordered_set<VarRef> variables;
			for (const auto& child : children) {
				auto iter = store.statement_key_map.find(child->getIdentifier());
				if (iter != store.statement_key_map.end()) {
					variables.insert(iter->second.begin(), iter->second.end());
				}
			}
			store.set(statement, variables);
		}
	}
}
