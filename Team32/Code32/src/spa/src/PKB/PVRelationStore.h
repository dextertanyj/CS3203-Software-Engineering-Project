#ifndef SPA_PVRELATIONSTORE_H
#define SPA_PVRELATIONSTORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "memory"

using namespace std;

template <class T>
class PKB::PVRelationStore {
public:
    PVRelationStore();
    void set(const ProcRef& proc, VarRef variable);
    void set(const ProcRef& proc, unordered_set<VarRef> variables);
    bool check(const ProcRef& proc, const VarRef& variable);
    VarRefSet getByProc(const ProcRef& proc);
    ProcRefSet getByVar(const VarRef& variable);
    void clear();

private:
    unordered_map<ProcRef, unordered_set<VarRef>> proc_key_map;
    unordered_map<VarRef, unordered_set<ProcRef>> variable_key_map;

    friend class UsesPRelation;
    friend class ModifiesPRelation;
};

#endif //SPA_PVRELATIONSTORE_H
