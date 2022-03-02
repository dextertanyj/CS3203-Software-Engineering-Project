#include "PKB/ModifiesPRelation.h"

void PKB::ModifiesPRelation::optimize(const TransitiveRelationStore<ProcRef, ProcedureInfo, CallRelation> &call_store,
                                      PVRelationStore<ModifiesPRelation> &store, SVRelationStore<ModifiesSRelation> &modifies_s_store,
                                      TopologicalSort<ProcedureInfo>& call_graph) {
    vector<shared_ptr<ProcedureInfo>> order = call_graph.get();
    for (auto itr = order.rbegin(); itr != order.rend(); itr++) {
        vector<shared_ptr<StmtInfo>> proc_stmts = itr->get()->getStatements();
        VarRefSet variables;
        for (const auto &stmt_info: proc_stmts) {
            VarRefSet variable_set = modifies_s_store.getByStmt(stmt_info->getIdentifier());
            variables.insert(variable_set.begin(), variable_set.end());
        }
        ProcRef proc = itr->get()->getIdentifier();
        unordered_set<shared_ptr<ProcedureInfo>> proc_reverse_transitive = call_store.getReverseTransitive(proc);
        for (const auto& proc_info : proc_reverse_transitive) {
            ProcRef proc_ref = proc_info->getIdentifier();
            VarRefSet variable_set = store.getByProc(proc_ref);
            variables.insert(variable_set.begin(), variable_set.end());
        }
        store.set(proc, variables);
    }
}
