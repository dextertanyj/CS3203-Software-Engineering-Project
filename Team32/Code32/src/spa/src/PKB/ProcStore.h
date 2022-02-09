#ifndef SPA_PROCSTORE_H
#define SPA_PROCSTORE_H

#include "Common/TypeDefs.h"
#include <unordered_map>

using namespace std;

struct ProcRelation {
    vector<shared_ptr<StmtInfo>> idxList;
    shared_ptr<StmtInfo> callStmt;
};

class ProcStore {
public:
    ProcStore();
    void setProc(ProcRef, vector<shared_ptr<StmtInfo>>);
    void setCall(shared_ptr<StmtInfo>, ProcRef);
    vector<shared_ptr<StmtInfo>> getStmtsByProc(ProcRef);
    vector<shared_ptr<StmtInfo>> getStmtsByCall(shared_ptr<StmtInfo>);
    ProcRef getProcByStmt(shared_ptr<StmtInfo>);
    void clear();
private:
    unordered_map<ProcRef, ProcRelation> procMap;
    unordered_map<StmtRef, ProcRef> stmtToProcMap;
};

#endif
