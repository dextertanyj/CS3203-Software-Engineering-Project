#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "Common/TypeDefs.h"
#include "PKB/AssignStore.h"
#include "PKB/FollowsPKB.h"
#include "PKB/Modifies.h"
#include "PKB/ParentPKB.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"

using namespace std;

class PKB {
public:
	PKB();

	// Set methods called by Source processor
	void setFollows(StmtRef, StmtRef);
	void setParent(StmtRef, StmtRef);
	void setStmtType(StmtRef, StmtType);
	void setUses(StmtRef, VarRef);
	void setModifies(StmtRef, VarRef);
	void setAssign(StmtRef, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree);

	// Get methods called by PQL

	// General get methods
	StmtInfoPtrSet getStatements();
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
	bool patternExists(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded);
	StmtInfoList getStmtsWithPattern(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded);
	StmtInfoList getStmtsWithPatternLHS(VarRef varName);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(Common::ArithmeticProcessor::ArithmeticExpression e,
	                                                                  bool isRHSExactMatchNeeded);
	// Others
	void clear();
	void populateComplexRelations();

	// For testing
	unordered_map<StmtRef, shared_ptr<StmtInfo>> getStmtInfoMap();

private:
	StatementStore statement_store;
	StatementRelationStore<ParentPKB> parent_store;
	StatementRelationStore<FollowsPKB> follows_store;
	SVRelationStore<Uses> uses_store;
	SVRelationStore<Modifies> modifies_store;
	AssignStore assign_store;
};
