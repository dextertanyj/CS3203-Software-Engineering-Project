#include "PKB/UsesPRelation.h"

void PKB::UsesPRelation::optimize(const Types::ProcedureStore& procStore,
                                  PVRelationStore<UsesPRelation>& store, SVRelationStore<UsesSRelation>& uses_s_store) {
	for (const auto& proc_info : procStore.getAll()) {
		vector<shared_ptr<StmtInfo>> proc_stmt_list = proc_info->getStatements();
		VarRefSet variables;
		for (const auto& statement : proc_stmt_list) {
			VarRefSet variable_set = uses_s_store.getByStmt(statement->getIdentifier());
			variables.insert(variable_set.begin(), variable_set.end());
		}
		store.set(proc_info->getIdentifier(), variables);
	}
}