#ifndef SPA_SRC_PKB_STORAGEACCESSINTERFACE_H
#define SPA_SRC_PKB_STORAGEACCESSINTERFACE_H

#include "PKB/PKB.h"

class PKB::StorageAccessInterface {
public:
	// General get methods
	virtual StmtInfoPtrSet getStatements() = 0;
	virtual VarRefSet getVariables() = 0;
	virtual unordered_set<ConstVal> getConstants() = 0;
	virtual unordered_set<ProcRef> getProcedures() = 0;

	// Parent get methods
	virtual bool checkParent(StmtRef parent, StmtRef child) = 0;
	virtual bool checkParentStar(StmtRef parent, StmtRef child) = 0;
	virtual shared_ptr<StmtInfo> getParent(StmtRef index) = 0;
	virtual StmtInfoPtrSet getParentStar(StmtRef index) = 0;
	virtual StmtInfoPtrSet getChildren(StmtRef index) = 0;
	virtual StmtInfoPtrSet getChildStar(StmtRef index) = 0;

	// Follow get methods
	virtual bool checkFollows(StmtRef front, StmtRef back) = 0;
	virtual bool checkFollowsStar(StmtRef front, StmtRef back) = 0;
	virtual shared_ptr<StmtInfo> getFollower(StmtRef index) = 0;
	virtual StmtInfoPtrSet getFollowerStar(StmtRef index) = 0;
	virtual shared_ptr<StmtInfo> getPreceding(StmtRef index) = 0;
	virtual StmtInfoPtrSet getPrecedingStar(StmtRef index) = 0;

	// Use get methods
	virtual bool checkUses(StmtRef index, const VarRef& name) = 0;
	virtual bool checkUses(const ProcRef& procedure_name, const VarRef& variable_name) = 0;
	virtual StmtInfoPtrSet getStmtUsesByVar(const VarRef& name) = 0;
	virtual ProcRefSet getProcUsesByVar(const VarRef& name) = 0;
	virtual VarRefSet getUsesByStmt(StmtRef index) = 0;
	virtual VarRefSet getUsesByProc(const ProcRef& name) = 0;

	// Modify get methods
	virtual bool checkModifies(StmtRef index, const VarRef& name) = 0;
	virtual bool checkModifies(const ProcRef& procedure_name, const VarRef& variable_name) = 0;
	virtual StmtInfoPtrSet getStmtModifiesByVar(const VarRef& name) = 0;
	virtual ProcRefSet getProcModifiesByVar(const VarRef& name) = 0;
	virtual VarRefSet getModifiesByStmt(StmtRef index) = 0;
	virtual VarRefSet getModifiesByProc(const ProcRef& name) = 0;

	// Assign get methods
	virtual bool patternExists(const VarRef& name, const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) = 0;
	virtual StmtInfoPtrSet getStmtsWithPattern(const VarRef& name, const Common::ExpressionProcessor::Expression& expression,
	                                           bool is_exact_match) = 0;
	virtual StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& name) = 0;
	virtual StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) = 0;

	// Calls get methods
	virtual bool checkCalls(const ProcRef& caller, const ProcRef& callee) = 0;
	virtual bool checkCallsStar(const ProcRef& caller, const ProcRef& callee) = 0;
	virtual ProcRefSet getCallee(const ProcRef& caller) = 0;
	virtual ProcRefSet getCalleeStar(const ProcRef& caller) = 0;
	virtual ProcRefSet getCaller(const ProcRef& callee) = 0;
	virtual ProcRefSet getCallerStar(const ProcRef& callee) = 0;

	// CFG Next Methods
	virtual bool checkNext(StmtRef first, StmtRef second) = 0;
	virtual bool checkNextStar(StmtRef first, StmtRef second) = 0;
	virtual StmtInfoPtrSet getNext(StmtRef first) = 0;
	virtual StmtInfoPtrSet getNextStar(StmtRef node_ref) = 0;
	virtual StmtInfoPtrSet getPrevious(StmtRef second) = 0;
	virtual StmtInfoPtrSet getPreviousStar(StmtRef node_ref) = 0;

	// CFG Affects Methods
	virtual bool checkAffects(StmtRef first, StmtRef second) = 0;
	virtual bool checkAffectsStar(StmtRef first, StmtRef second) = 0;
	virtual StmtInfoPtrSet getAffects(StmtRef first) = 0;
	virtual StmtInfoPtrSet getAffectsStar(StmtRef first) = 0;
	virtual StmtInfoPtrSet getAffected(StmtRef second) = 0;
	virtual StmtInfoPtrSet getAffectedStar(StmtRef second) = 0;

	// Control Variable get methods
	virtual bool checkIfControl(StmtRef index, VarRef name) = 0;
	virtual bool checkWhileControl(StmtRef index, VarRef name) = 0;
	virtual VarRefSet getIfControlVar(StmtRef index) = 0;
	virtual VarRefSet getWhileControlVar(StmtRef index) = 0;
	virtual StmtInfoPtrSet getIfControlStmt(VarRef name) = 0;
	virtual StmtInfoPtrSet getWhileControlStmt(VarRef name) = 0;

	// Attribute get methods
	virtual ProcRef getCalledProcedure(StmtRef index) = 0;

	// CFG methods
	virtual void resetCFGCache() = 0;

	virtual ~StorageAccessInterface() = default;
};

#endif  // SPA_SRC_PKB_STORAGEACCESSINTERFACE_H
