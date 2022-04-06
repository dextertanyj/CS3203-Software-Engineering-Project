#ifndef SPA_SRC_PKB_PVRELATIONSTORE_H
#define SPA_SRC_PKB_PVRELATIONSTORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "memory"

template <class T>
class PKB::PVRelationStore {
public:
	PVRelationStore();
	void set(const ProcRef& proc, const VarRef& variable);
	void set(const ProcRef& proc, const std::unordered_set<VarRef>& variables);
	bool check(const ProcRef& proc, const VarRef& variable);
	VarRefSet getByProc(const ProcRef& proc);
	ProcRefSet getByVar(const VarRef& variable);
	void clear();

private:
	std::unordered_map<ProcRef, std::unordered_set<VarRef>> proc_key_map;
	std::unordered_map<VarRef, std::unordered_set<ProcRef>> variable_key_map;

	friend class UsesPRelation;
	friend class ModifiesPRelation;
};

#endif  // SPA_SRC_PKB_PVRELATIONSTORE_H
