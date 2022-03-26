#include "PKB/UsesSRelation.h"

#include <algorithm>

#include "PKB/SVRelationStore.tpp"
#include "PKB/TopologicalSort.tpp"

bool PKB::UsesSRelation::validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement, const VarRef& variable) {
	StmtRef idx = statement->getIdentifier();
	if (statement->getType() == StmtType::Read) {
		throw invalid_argument("Read statements cannot use a variable");
	}
	if (statement->getType() == StmtType::WhileStmt || statement->getType() == StmtType::IfStmt || statement->getType() == StmtType::Call ||
	    statement->getType() == StmtType::Assign) {
		return true;
	}
	auto statement_iter = store->statement_key_map.find(idx);
	if (statement_iter == store->statement_key_map.end()) {
		return true;
	}
	return !(any_of(statement_iter->second.begin(), statement_iter->second.end(),
	                [variable](const VarRef& existing_var) { return existing_var != variable; }));
}

bool PKB::UsesSRelation::validate(SVRelationStore<UsesSRelation>* store, const shared_ptr<StmtInfo>& statement,
                                  const VarRefSet& variables) {
	StmtRef idx = statement->getIdentifier();
	if (statement->getType() == StmtType::Read) {
		throw invalid_argument("Read statements cannot use a variable");
	}
	if (statement->getType() == StmtType::WhileStmt || statement->getType() == StmtType::IfStmt || statement->getType() == StmtType::Call ||
	    statement->getType() == StmtType::Assign || variables.empty()) {
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

void PKB::UsesSRelation::optimize(Types::ParentStore& parent_store, CallsStatementStore& call_store, Types::ProcedureStore& proc_store,
                                  TopologicalSort<ProcedureInfo>& topo_order, SVRelationStore<UsesSRelation>& store) {
	// Start optimization from the lowest level in the DAG.
	vector<shared_ptr<ProcedureInfo>> order = topo_order.get();
	for (auto proc_iterator = order.rbegin(); proc_iterator != order.rend(); ++proc_iterator) {
		vector<shared_ptr<StmtInfo>> stmts_in_proc = proc_iterator->get()->getStatements();
		// For any procedure, we must process the call statements first before propagating the conditional statements.
		std::for_each(stmts_in_proc.begin(), stmts_in_proc.end(),
		              [&call_store, &proc_store, &store](shared_ptr<StmtInfo> info) { optimizeCall(info, call_store, proc_store, store); });
		std::for_each(stmts_in_proc.begin(), stmts_in_proc.end(),
		              [&parent_store, &store](shared_ptr<StmtInfo> info) { optimizeConditional(info, parent_store, store); });
	}
}

void PKB::UsesSRelation::optimizeCall(const shared_ptr<StmtInfo>& statement, CallsStatementStore& call_store,
                                      Types::ProcedureStore& proc_store, SVRelationStore<UsesSRelation>& store) {
	if (statement->getType() != StmtType::Call) {
		return;
	}
	// Need to access CallStatementStore to get the statements used in the called procedure.
	ProcRef called_proc = call_store.getProcedure(statement);
	shared_ptr<ProcedureInfo> proc_info = proc_store.get(called_proc);
	vector<shared_ptr<StmtInfo>> stmts_in_called_proc = proc_info->getStatements();
	StmtInfoPtrSet stmt_list(stmts_in_called_proc.begin(), stmts_in_called_proc.end());
	storeUsedVars(statement, stmt_list, store);
}

void PKB::UsesSRelation::optimizeConditional(const shared_ptr<StmtInfo>& statement, Types::ParentStore& parent_store,
                                             SVRelationStore<UsesSRelation>& store) {
	if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
		return;
	}
	// For conditional statements, need to look at the child* statements for uses statements.
	auto children = parent_store.getReverseTransitive(statement->getIdentifier());
	storeUsedVars(statement, children, store);
}

void PKB::UsesSRelation::storeUsedVars(shared_ptr<StmtInfo> stmt_key, StmtInfoPtrSet stmt_list,
                                       PKB::SVRelationStore<PKB::UsesSRelation>& store) {
	VarRefSet variables;
	for (const auto& stmt : stmt_list) {
		auto iter = store.statement_key_map.find(stmt->getIdentifier());
		// If child statement uses a variable, then we must record it in the parent conditional statement.
		if (iter != store.statement_key_map.end()) {
			variables.insert(iter->second.begin(), iter->second.end());
		}
	}
	if (!variables.empty()) {
		store.set(stmt_key, variables);
	}
}
