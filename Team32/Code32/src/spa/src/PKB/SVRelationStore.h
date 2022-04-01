#ifndef SPA_SRC_PKB_SVRELATIONSTORE_H
#define SPA_SRC_PKB_SVRELATIONSTORE_H

#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "memory"

using namespace std;

template<class T>
class PKB::SVRelationStore {
public:
    SVRelationStore();

    void set(shared_ptr<StmtInfo> statement, VarRef variable);

    void set(shared_ptr<StmtInfo> statement, VarRefSet variables);

    bool check(StmtRef index, const VarRef &variable);

    VarRefSet getByStmt(StmtRef index);

    StmtInfoPtrSet getByVar(const VarRef &variable);

    void clear();

private:
    unordered_map<StmtRef, VarRefSet> statement_key_map;
    unordered_map<VarRef, StmtInfoPtrSet> variable_key_map;

    friend class UsesSRelation;

    friend class ModifiesSRelation;
};

#endif  // SPA_SRC_PKB_SVRELATIONSTORE_H
