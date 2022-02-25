#ifndef SPA_SRC_PKB_PROCEDURESTORE_H
#define SPA_SRC_PKB_PROCEDURESTORE_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

using namespace std;

class PKB::ProcedureStore {
public:
	ProcedureStore();
	void setProc(const ProcRef&, const vector<shared_ptr<StmtInfo>>&);
	void setCall(const shared_ptr<StmtInfo>&, const ProcRef&);
	vector<shared_ptr<StmtInfo>> getStmtsByProc(const ProcRef&);
	ProcRef getProcByStmt(const shared_ptr<StmtInfo>&);
	ProcRef getProcByCall(const shared_ptr<StmtInfo>&);
	unordered_set<ProcRef> getProcListByStmtList(const unordered_set<shared_ptr<StmtInfo>>&);
	void clear();

private:
	unordered_map<ProcRef, ProcRelation> proc_map;
	unordered_map<StmtRef, ProcRef> stmt_to_proc_map;
};

#endif
