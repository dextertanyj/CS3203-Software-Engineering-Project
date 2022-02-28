#ifndef SPA_SRC_PKB_CALLRELATION_H
#define SPA_SRC_PKB_CALLRELATION_H

#include <memory>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"

class PKB::CallRelation {
public:
	explicit CallRelation(shared_ptr<ProcedureInfo> self);
	void insertForward(const shared_ptr<ProcedureInfo>& caller);
	void insertReverse(const shared_ptr<ProcedureInfo>& callee);
	void appendForwardTransitive(unordered_set<shared_ptr<ProcedureInfo>> new_callers);
	void appendReverseTransitive(unordered_set<shared_ptr<ProcedureInfo>> new_callees);

	[[nodiscard]] shared_ptr<ProcedureInfo> getSelf() const;
	[[nodiscard]] unordered_set<shared_ptr<ProcedureInfo>> getForward() const;
	[[nodiscard]] unordered_set<shared_ptr<ProcedureInfo>> getReverse() const;
	[[nodiscard]] unordered_set<shared_ptr<ProcedureInfo>> getForwardTransitive() const;
	[[nodiscard]] unordered_set<shared_ptr<ProcedureInfo>> getReverseTransitive() const;

private:
	shared_ptr<ProcedureInfo> self;
	unordered_set<shared_ptr<ProcedureInfo>> callers;
	unordered_set<shared_ptr<ProcedureInfo>> callees;
	unordered_set<shared_ptr<ProcedureInfo>> callers_transitive;
	unordered_set<shared_ptr<ProcedureInfo>> callees_transitive;
};

#endif  // SPA_SRC_PKB_CALLRELATION_H
