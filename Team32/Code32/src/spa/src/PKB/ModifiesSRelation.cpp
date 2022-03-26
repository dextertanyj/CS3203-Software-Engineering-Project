#include "PKB/ModifiesSRelation.h"

#include <algorithm>

#include "PKB/SVRelationStore.tpp"
#include "PKB/TopologicalSort.tpp"

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

void PKB::ModifiesSRelation::optimize(Types::ParentStore& parent_store, CallsStatementStore& call_store, Types::ProcedureStore& proc_store,
                                      TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<ModifiesSRelation>& store) {
	// Start optimization from the lowest level in the DAG.
	vector<shared_ptr<ProcedureInfo>> order = topo_order.get();
	for (auto proc_iterator = order.rbegin(); proc_iterator != order.rend(); ++proc_iterator) {
		vector<shared_ptr<StmtInfo>> stmts_in_proc = proc_iterator->get()->getStatements();
		// For any procedure, we must process the call statements first before propagating the conditional statements.
		std::for_each(stmts_in_proc.begin(), stmts_in_proc.end(), [&call_store, &proc_store, &store](const shared_ptr<StmtInfo>& info) {
			optimizeCall(info, call_store, proc_store, store);
		});
		std::for_each(stmts_in_proc.begin(), stmts_in_proc.end(),
		              [&parent_store, &store](const shared_ptr<StmtInfo>& info) { optimizeConditional(info, parent_store, store); });
	}
}

void PKB::ModifiesSRelation::optimizeCall(const shared_ptr<StmtInfo>& statement, CallsStatementStore& call_store,
                                          Types::ProcedureStore& proc_store, SVRelationStore<ModifiesSRelation>& store) {
	// Need to access CallStatementStore to get the statements modified in the called procedure.
	if (statement->getType() != StmtType::Call) {
		return;
	}
	ProcRef called_proc = call_store.getProcedure(statement);
	shared_ptr<ProcedureInfo> proc_info = proc_store.get(called_proc);
	vector<shared_ptr<StmtInfo>> stmts_in_called_proc = proc_info->getStatements();
	StmtInfoPtrSet stmts_set_in_proc(stmts_in_called_proc.begin(), stmts_in_called_proc.end());
	storeModifiedVars(statement, stmts_set_in_proc, store);
}

void PKB::ModifiesSRelation::optimizeConditional(const shared_ptr<StmtInfo>& statement, Types::ParentStore& parent_store,
                                                 SVRelationStore<ModifiesSRelation>& store) {
	if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
		return;
	}
	// For conditional statements, need to look at the child* statements for modify statements.
	auto children = parent_store.getReverseTransitive(statement->getIdentifier());
	storeModifiedVars(statement, children, store);
}

void PKB::ModifiesSRelation::storeModifiedVars(const shared_ptr<StmtInfo>& stmt_key, const StmtInfoPtrSet& stmt_list,
                                               PKB::SVRelationStore<PKB::ModifiesSRelation>& store) {
	VarRefSet variables;
	for (const auto& stmt : stmt_list) {
		auto iter = store.statement_key_map.find(stmt->getIdentifier());
		// If child statement modifies a variable, then we must record it in the parent conditional statement.
		if (iter != store.statement_key_map.end()) {
			variables.insert(iter->second.begin(), iter->second.end());
		}
	}
	if (!variables.empty()) {
		store.set(std::move(stmt_key), variables);
	}
}
