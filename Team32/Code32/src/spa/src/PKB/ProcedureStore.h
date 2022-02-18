#ifndef SPA_PROCEDURESTORE_H
#define SPA_PROCEDURESTORE_H

#include "Common/TypeDefs.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "iostream"

using namespace std;

struct ProcRelation {
    vector<shared_ptr<StmtInfo>> idxList;
    shared_ptr<StmtInfo> callStmt;
};

class ProcedureStore {
public:
	ProcedureStore();
    void setProc(ProcRef, vector<shared_ptr<StmtInfo>>);
    void setCall(shared_ptr<StmtInfo>, ProcRef);
    vector<shared_ptr<StmtInfo>> getStmtsByProc(ProcRef);
    ProcRef getProcByStmt(shared_ptr<StmtInfo>);
    ProcRef getProcByCall(shared_ptr<StmtInfo>);
    unordered_set<ProcRef> getProcListByStmtList(unordered_set<shared_ptr<StmtInfo>>);
    void clear();
private:
    unordered_map<ProcRef, ProcRelation> procMap;
    unordered_map<StmtRef, ProcRef> stmtToProcMap;
};

#endif