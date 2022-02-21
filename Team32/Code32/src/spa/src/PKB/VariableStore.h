#ifndef SPA_VARIABLESTORE_H
#define SPA_VARIABLESTORE_H

#include <unordered_set>

#include "PKB/PKB.h"
#include "Common/TypeDefs.h"

class PKB::VariableStore {
public:
	VariableStore();
	void insert(const VarRef& variable);
	void insert(const VarRefSet& variables);
	unordered_set<VarRef> getAll();

private:
	unordered_set<VarRef> store;
};

#endif  // SPA_VARIABLESTORE_H
