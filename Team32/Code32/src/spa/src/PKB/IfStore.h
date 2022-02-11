#ifndef SPA_IFSTORE_H
#define SPA_IFSTORE_H

#include "Common/TypeDefs.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>

using namespace std;

class IfStore {
public:
    IfStore();
    void populateIfStore(shared_ptr<StmtInfo> ifInfo, unordered_set<shared_ptr<StmtInfo>> stmtsInfo);
    unordered_set<shared_ptr<StmtInfo>> getIfsFromStmt(shared_ptr<StmtInfo> stmtInfo);
    unordered_set<shared_ptr<StmtInfo>> getStmtsFromIf(shared_ptr<StmtInfo> ifIndo);
    void clear();
private:
    unordered_map<StmtRef, unordered_set<shared_ptr<StmtInfo>>> ifToStmtsMap;
    unordered_map<StmtRef, unordered_set<shared_ptr<StmtInfo>>> stmtToIfsMap;
};

#endif //SPA_IFSTORE_H
