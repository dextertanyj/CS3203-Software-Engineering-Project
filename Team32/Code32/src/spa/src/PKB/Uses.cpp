#include "PKB/Uses.h"

#include "PKB/SVRelationStore.tpp"

void Uses::optimize(StatementStore& statement_store, StatementRelationStore<ParentPKB>& parent_store, SVRelationStore<Uses>& store) {
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

bool Uses::validate(SVRelationStore<Uses>* store, shared_ptr<StmtInfo> statement, VarRef variable) {
	StmtRef idx = statement->reference;
	if (statement->type == StmtType::Read) {
		throw "Read statements cannot use a variable";
	}
	if (statement->type == StmtType::WhileStmt || statement->type == StmtType::IfStmt) {
		return true;
	}
	auto statement_iter = store->statement_key_map.find(idx);
	if (statement_iter == store->statement_key_map.end()) {
		return true;
	}
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(), [variable](VarRef x) { return x == variable; }));
}