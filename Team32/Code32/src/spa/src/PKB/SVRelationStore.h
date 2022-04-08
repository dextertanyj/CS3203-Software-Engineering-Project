#ifndef SPA_SRC_PKB_SVRELATIONSTORE_H
#define SPA_SRC_PKB_SVRELATIONSTORE_H

#include <unordered_map>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

template <class T>
class PKB::SVRelationStore {
public:
	SVRelationStore();
	void set(StmtInfoPtr statement, VarRef variable);
	void set(StmtInfoPtr statement, VarRefSet variables);
	bool check(StmtRef index, const VarRef& variable);
	VarRefSet getByStmt(StmtRef index);
	StmtInfoPtrSet getByVar(const VarRef& variable);
	void clear();

private:
	std::unordered_map<StmtRef, VarRefSet> statement_key_map;
	std::unordered_map<VarRef, StmtInfoPtrSet> variable_key_map;

	friend class UsesSRelation;
	friend class ModifiesSRelation;
};

#endif  // SPA_SRC_PKB_SVRELATIONSTORE_H
