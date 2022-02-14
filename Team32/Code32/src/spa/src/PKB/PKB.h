#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "AssignStore.h"
#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "Common/TypeDefs.h"
#include "FollowStore.h"
#include "ModifyStore.h"
#include "ParentStore.h"
#include "UseStore.h"

using namespace std;

class PKB {
public:
	PKB();

	// Set methods called by Source processor
	void setFollows(StmtRef, StmtRef);
	void setParent(StmtRef, StmtRef);
	void setProc(ProcRef, StmtRefList);
	void setStmtType(StmtRef, StmtType);
	void setUses(StmtRef, VarRef);
	void setModifies(StmtRef, VarRef);
	void setAssign(StmtRef, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree);
	void setCall(StmtRef, ProcRef);

	// Get methods called by PQL

	// General get methods
	StmtInfoList getStatements();
	unordered_map<StmtRef, shared_ptr<StmtInfo>> getStmtInfoMap();

	// Parent get methods
	shared_ptr<StmtInfo> getParent(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getChildren(StmtRef);
	bool checkParents(StmtRef, StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getParentStar(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getChildStar(StmtRef);

	// Follow get methods
	shared_ptr<StmtInfo> getPreceding(StmtRef stmt);
	shared_ptr<StmtInfo> getFollower(StmtRef);
	bool checkFollows(StmtRef, StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getFollowerStar(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getPrecedingStar(StmtRef);

	// Use get methods
	bool checkUses(StmtRef, VarRef);
	unordered_set<shared_ptr<StmtInfo>> getUsesByVar(VarRef);
	unordered_set<VarRef> getUsesByStmt(StmtRef);

	// Modify get methods
	bool checkModifies(StmtRef, VarRef);
	unordered_set<shared_ptr<StmtInfo>> getModifiesByVar(VarRef);
	unordered_set<VarRef> getModifiesByStmt(StmtRef);

	// Assign get methods
	bool patternExists(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded);
	StmtInfoList getStmtsWithPattern(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded);
	StmtInfoList getStmtsWithPatternLHS(VarRef varName);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(Common::ArithmeticProcessor::ArithmeticExpression e,
                                                                      bool isRHSExactMatchNeeded);

	// Others
	void clear();
	void populateComplexRelations();

private:
	ParentStore parentStore;
	FollowStore followStore;
	UseStore useStore;
	ModifyStore modifyStore;
	AssignStore assignStore;
	unordered_map<StmtRef, shared_ptr<StmtInfo>> stmtInfoMap;  // Stores StmtInfo for a particular StmtRef.
};
