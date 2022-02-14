#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ParentStore.h"
#include "FollowStore.h"
#include "UseStore.h"
#include "ModifyStore.h"
#include "AssignStore.h"
#include "Common/TypeDefs.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/Expression.h"

using namespace std;

class PKB {
public:
    PKB();

	// Set methods called by Source processor
	void setFollows(StmtRef, StmtRef);
	void setParent(StmtRef, StmtRef);
	void setStmtType(StmtRef, StmtType);
	void setConstant(int);
	void setConstant(unordered_set<int>);
	void setUses(StmtRef, VarRef);
	void setModifies(StmtRef, VarRef);
	void setUses(StmtRef, VarRefSet);
	void setModifies(StmtRef, VarRefSet);
	void setAssign(StmtRef, VarRef variableLHS, Common::ExpressionProcessor::Expression opTree);

	// Get methods called by PQL

	// General get methods
	StmtInfoPtrSet getStatements();
	VarRefSet getVariables();
	unordered_set<int> getConstants();

	// Parent get methods
	shared_ptr<StmtInfo> getParent(StmtRef);
	StmtInfoPtrSet getChildren(StmtRef);
	bool checkParents(StmtRef, StmtRef);
	StmtInfoPtrSet getParentStar(StmtRef);
	StmtInfoPtrSet getChildStar(StmtRef);

	// Follow get methods
	shared_ptr<StmtInfo> getPreceding(StmtRef stmt);
	shared_ptr<StmtInfo> getFollower(StmtRef);
	bool checkFollows(StmtRef, StmtRef);
	StmtInfoPtrSet getFollowerStar(StmtRef);
	StmtInfoPtrSet getPrecedingStar(StmtRef);

	// Use get methods
	bool checkUses(StmtRef, VarRef);
	StmtInfoPtrSet getUsesByVar(VarRef);
	VarRefSet getUsesByStmt(StmtRef);

	// Modify get methods
	bool checkModifies(StmtRef, VarRef);
	StmtInfoPtrSet getModifiesByVar(VarRef);
	VarRefSet getModifiesByStmt(StmtRef);

	// Assign get methods
	bool patternExists(VarRef varName, Common::ExpressionProcessor::Expression e, bool isRHSExactMatchNeeded);
	StmtInfoPtrSet getStmtsWithPattern(VarRef varName, Common::ExpressionProcessor::Expression e, bool isRHSExactMatchNeeded);
	StmtInfoPtrSet getStmtsWithPatternLHS(VarRef varName);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(Common::ExpressionProcessor::Expression e,
	                                                                  bool isRHSExactMatchNeeded);


    // Others
    void clear();
    void setNumStatements(int size);
    void populateComplexRelations();

private:
    ParentStore parentStore;
    FollowStore followStore;
    UseStore useStore;
    ModifyStore modifyStore;
    AssignStore assignStore;
    unordered_map<ProcRef, vector<StmtRef>> procMap;
    unordered_map<StmtRef, StmtType> typeMap;
    int numStatements;
    void checkInvalidStmts(StmtRef stmtNo1, StmtRef stmtNo2);
    void checkInvalidStmt(StmtRef stmtNo1);
};
