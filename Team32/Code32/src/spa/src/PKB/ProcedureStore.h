#ifndef SPA_SRC_PKB_PROCEDURESTORE_H
#define SPA_SRC_PKB_PROCEDURESTORE_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"

using namespace std;

class PKB::ProcedureStore {
public:
	ProcedureStore();
	void insert(const ProcRef& procedure, vector<shared_ptr<StmtInfo>> statements);
	[[nodiscard]] shared_ptr<ProcedureInfo> get(const ProcRef& procedure) const;
	[[nodiscard]] unordered_set<ProcRef> getAll() const;
	[[nodiscard]] unordered_set<shared_ptr<ProcedureInfo>> getAllInfo() const;
	void clear();

private:
	unordered_map<ProcRef, shared_ptr<ProcedureInfo>> store;
};

#endif
