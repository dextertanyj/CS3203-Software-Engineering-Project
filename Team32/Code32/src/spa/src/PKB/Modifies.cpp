#include "PKB/Modifies.h"

#include "PKB/SVRelationStore.tpp"

bool Modifies::validate(SVRelationStore<Modifies>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable) {
	StmtRef idx = statement->reference;
	if (statement->type == StmtType::Print) {
		throw invalid_argument("Print statements cannot modify a variable");
	}
	if (statement->type == StmtType::WhileStmt || statement->type == StmtType::IfStmt || statement->type == StmtType::Call) {
		return true;
	}
	auto statement_iter = store->statement_key_map.find(idx);
	if (statement_iter == store->statement_key_map.end()) {
		return true;
	}
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(), [variable](const VarRef& x) { return x != variable; }));
}

bool Modifies::validate(SVRelationStore<Modifies>* store, const shared_ptr<StmtInfo>& statement, const VarRefSet& variables) {
	StmtRef idx = statement->reference;
	if (statement->type == StmtType::Print) {
		throw invalid_argument("Print statements cannot modify a variable");
	}
	if (statement->type == StmtType::WhileStmt || statement->type == StmtType::IfStmt || statement->type == StmtType::Call) {
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
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(), [variable](const VarRef& x) { return x == variable; }));
}

void Modifies::optimize(StatementStore& statement_store, StatementRelationStore<ParentPKB>& parent_store,
                        SVRelationStore<Modifies>& store) {
	for (const auto& statement : statement_store.getAll()) {
		if (statement->type == StmtType::IfStmt || statement->type == StmtType::WhileStmt) {
			auto children = parent_store.getReverseTransitive(statement->reference);
			unordered_set<VarRef> variables;
			for (auto child : children) {
				auto iter = store.statement_key_map.find(child->reference);
				if (iter != store.statement_key_map.end()) {
					variables.insert(iter->second.begin(), iter->second.end());
				}
			}
			store.set(statement, variables);
		}
	}
}
