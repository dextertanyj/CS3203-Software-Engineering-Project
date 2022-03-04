#include "PKB/ModifiesPRelation.h"

void PKB::ModifiesPRelation::optimize(const Types::ProcedureStore &proc_store, PVRelationStore<ModifiesPRelation> &store,
                                      SVRelationStore<ModifiesSRelation> &modifies_s_store) {
	for (const auto &proc_info : proc_store.getAll()) {
		vector<shared_ptr<StmtInfo>> proc_stmt_list = proc_info->getStatements();
		VarRefSet variables;
		for (const auto &statement : proc_stmt_list) {
			VarRefSet variable_set = modifies_s_store.getByStmt(statement->getIdentifier());
			variables.insert(variable_set.begin(), variable_set.end());
		}
		store.set(proc_info->getIdentifier(), variables);
	}
}
