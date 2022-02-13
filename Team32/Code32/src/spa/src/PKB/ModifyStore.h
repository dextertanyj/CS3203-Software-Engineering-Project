#ifndef INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

#pragma once

#include <stdio.h>

#include <string>
#include <unordered_map>
#include <unordered_set>


#include "PKB/ParentStore.h"
#include "../Common/TypeDefs.h"
#include "iostream"
#include "memory"

using namespace std;

class ModifyStore {
public:
	ModifyStore();
	void setModify(shared_ptr<StmtInfo>, VarRef);
	void setModify(shared_ptr<StmtInfo>, unordered_set<VarRef>);
	bool checkModifies(StmtRef, VarRef);
	unordered_set<VarRef> getModifiesByStmt(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getModifiesByVar(VarRef);
	void optimize(unordered_map<StmtRef, shared_ptr<StmtInfo>>, ParentStore& parent_store);
	void clear();

private:
	unordered_map<StmtRef, unordered_set<VarRef>> stmtToVarMap;
	unordered_map<VarRef, unordered_set<shared_ptr<StmtInfo>>> varToStmtMap;
};

#endif  // INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H
