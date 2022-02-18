#ifndef INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H

#pragma once

#include <stdio.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../Common/TypeDefs.h"
#include "iostream"
#include "memory"

using namespace std;

template <class T>
class SVRelationStore {
public:
	SVRelationStore();
	void set(shared_ptr<StmtInfo> statement, VarRef variable);
	void set(shared_ptr<StmtInfo> statement, unordered_set<VarRef> variables);
	bool check(StmtRef index, VarRef variable);
	unordered_set<VarRef> getByStmt(StmtRef index);
	unordered_set<shared_ptr<StmtInfo>> getByVar(VarRef variable);
	void clear();

private:
	unordered_map<StmtRef, unordered_set<VarRef>> statement_key_map;
	unordered_map<VarRef, unordered_set<shared_ptr<StmtInfo>>> variable_key_map;

	friend class Uses;
	friend class Modifies;
};

#endif  // INC_21S2_CP_SPA_TEAM_32_MODIFYSTORE_H