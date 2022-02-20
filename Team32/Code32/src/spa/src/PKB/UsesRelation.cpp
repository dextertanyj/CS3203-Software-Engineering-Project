#include "PKB/UsesRelation.h"

#include <algorithm>

#include "PKB/SVRelationStore.tpp"

bool UsesRelation::validate(SVRelationStore<UsesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable) {
	StmtRef idx = statement->reference;
	if (statement->type == StmtType::Read) {
		throw invalid_argument("Read statements cannot use a variable");
	}
	if (statement->type == StmtType::WhileStmt || statement->type == StmtType::IfStmt || statement->type == StmtType::Call ||
	    statement->type == StmtType::Assign) {
		return true;
	}
	auto statement_iter = store->statement_key_map.find(idx);
	if (statement_iter == store->statement_key_map.end()) {
		return true;
	}
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(), [variable](const VarRef& existing_var) { return existing_var != variable; }));
}

bool UsesRelation::validate(SVRelationStore<UsesRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables) {
	StmtRef idx = statement->reference;
	if (statement->type == StmtType::Read) {
		throw invalid_argument("Read statements cannot use a variable");
	}
	if (statement->type == StmtType::WhileStmt || statement->type == StmtType::IfStmt || statement->type == StmtType::Call ||
	    statement->type == StmtType::Assign || variables.empty()) {
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
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(), [variable](const VarRef& existing_var) { return existing_var == variable; }));
}

void UsesRelation::optimize(StatementStore& statement_store, StatementRelationStore<ParentRelation>& parent_store,
                            SVRelationStore<UsesRelation>& store) {
	for (const auto& statement : statement_store.getAll()) {
		if (statement->type == StmtType::IfStmt || statement->type == StmtType::WhileStmt) {
			auto children = parent_store.getReverseTransitive(statement->reference);
			unordered_set<VarRef> variables;
			for (const auto& child : children) {
				auto iter = store.statement_key_map.find(child->reference);
				if (iter != store.statement_key_map.end()) {
					variables.insert(iter->second.begin(), iter->second.end());
				}
			}
			store.set(statement, variables);
		}
	}
}
