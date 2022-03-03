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

void PKB::UsesSRelation::optimize(StatementRelationStore<ParentRelation>& parent_store, CallStatementStore& call_store,
                                  Types::ProcedureStore& proc_store, TopologicalSort<ProcedureInfo>& topo_order,
                                  SVRelationStore<UsesSRelation>& store) {
	vector<shared_ptr<ProcedureInfo>> order = topo_order.get();
	// Start optimization from the lowest level in the DAG.
	for (auto proc_iterator = order.rbegin(); proc_iterator != order.rend(); ++proc_iterator) {
		vector<shared_ptr<StmtInfo>> stmts_in_proc = proc_iterator->get()->getStatements();
		for (const auto& statement : stmts_in_proc) {
			VarRefSet variables;
			if (statement->getType() == StmtType::Call) {
				variables = optimizeCall(statement, call_store, proc_store, store);
			} else if (statement->getType() == StmtType::IfStmt || statement->getType() == StmtType::WhileStmt) {
				variables = optimizeConditional(statement, parent_store, store);
			}
			if (!variables.empty()) {
				store.set(statement, variables);
			}
		}
	}
}

VarRefSet PKB::UsesSRelation::optimizeCall(const shared_ptr<StmtInfo>& statement, CallStatementStore& call_store,
                                           Types::ProcedureStore& proc_store, SVRelationStore<UsesSRelation>& store) {
	// Need to access CallStatementStore to get the statements modified in the called procedure.
	VarRefSet variables;
	ProcRef called_proc = call_store.getProcedure(statement);
	shared_ptr<ProcedureInfo> proc_info = proc_store.get(called_proc);
	vector<shared_ptr<StmtInfo>> stmts_in_called_proc = proc_info->getStatements();
	for (const auto& stmt_in_called_proc : stmts_in_called_proc) {
		auto iter = store.statement_key_map.find(stmt_in_called_proc->getIdentifier());
		// If statement modifies a variable, then we must record it in the calling statement.
		if (iter != store.statement_key_map.end()) {
			variables.insert(iter->second.begin(), iter->second.end());
		}
	}
	return variables;
}

VarRefSet PKB::UsesSRelation::optimizeConditional(const shared_ptr<StmtInfo>& statement,
                                                  StatementRelationStore<ParentRelation>& parent_store,
                                                  SVRelationStore<UsesSRelation>& store) {
	// For conditional statements, need to look at the child* statements for modify statements.
	VarRefSet variables;
	auto children = parent_store.getReverseTransitive(statement->getIdentifier());
	for (const auto& child : children) {
		auto iter = store.statement_key_map.find(child->getIdentifier());
		// If child statement modifies a variable, then we must record it in the parent conditional statement.
		if (iter != store.statement_key_map.end()) {
			variables.insert(iter->second.begin(), iter->second.end());
		}
	}
	return variables;
}
