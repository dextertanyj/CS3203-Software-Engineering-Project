#include "Uses.h"
#include "SVRelationStore.tpp"

void Uses::optimize(const StatementStore& statement_store, StatementRelationStore<ParentPKB>& parent_store,
                        SVRelationStore<Uses>& store) {
	for (auto statement : statement_store) {
		if (statement.second->type == StmtType::IfStmt || statement.second->type == StmtType::WhileStmt) {
			auto children = parent_store.getReverseTransitive(statement.second->reference);
			unordered_set<VarRef> variables;
			for (auto child : children) {
				auto iter = store.statement_key_map.find(child->reference);
				if (iter != store.statement_key_map.end()) {
					variables.insert(iter->second.begin(), iter->second.end());
				}
			}
			store.set(statement.second, variables);
		}
	}
}

bool Uses::validate(SVRelationStore<Uses>* store, shared_ptr<StmtInfo> statement, VarRef variable) {
	StmtRef idx = statement->reference;
	store->statement_key_map.find(idx);
	store->variable_key_map.find(variable);
	return true;
}