#ifndef SPA_VARIABLESTORE_H
#define SPA_VARIABLESTORE_H

#include <unordered_set>

#include "Common/TypeDefs.h"

class VariableStore {
public:
	VariableStore();
	void insert(const VarRef& value);
	void insert(const VarRefSet& values);
	unordered_set<VarRef> getAll();

private:
	unordered_set<VarRef> store;
};

#endif  // SPA_VARIABLESTORE_H
