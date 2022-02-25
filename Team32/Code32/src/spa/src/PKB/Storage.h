#ifndef SPA_SRC_PKB_STORAGE_H
#define SPA_SRC_PKB_STORAGE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/AssignStore.h"
#include "PKB/ConstantStore.h"
#include "PKB/FollowsRelation.h"
#include "PKB/ModifiesRelation.h"
#include "PKB/PKB.h"
#include "PKB/ParentRelation.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"
#include "PKB/VariableStore.h"

using namespace std;

class PKB::Storage {
public:
	Storage();

	// Set methods called by Source processor
	void setProc(ProcRef, StmtRef, StmtRef);
	void setCall(StmtRef, ProcRef);
	void setFollows(StmtRef, StmtRef);
	void setParent(StmtRef, StmtRef);
	void setStmtType(StmtRef, StmtType);
	void setConstant(ConstVal);
	void setConstant(const unordered_set<ConstVal>&);
	void setUses(StmtRef, VarRef);
	void setModifies(StmtRef, VarRef);
	void setUses(StmtRef, VarRefSet);
	void setModifies(StmtRef, VarRefSet);
	void setAssign(StmtRef, VarRef variable, Common::ExpressionProcessor::Expression expression);

	// Get methods called by PQL

	// General get methods
	StmtInfoPtrSet getStatements();
	VarRefSet getVariables();
	unordered_set<ConstVal> getConstants();

	// Parent get methods
	shared_ptr<StmtInfo> getParent(StmtRef);
	StmtInfoPtrSet getChildren(StmtRef);
	bool checkParents(StmtRef, StmtRef);
	StmtInfoPtrSet getParentStar(StmtRef);
	StmtInfoPtrSet getChildStar(StmtRef);

	// Follow get methods
	shared_ptr<StmtInfo> getPreceding(StmtRef);
	shared_ptr<StmtInfo> getFollower(StmtRef);
	bool checkFollows(StmtRef, StmtRef);
	StmtInfoPtrSet getFollowerStar(StmtRef);
	StmtInfoPtrSet getPrecedingStar(StmtRef);

	// Use get methods
	bool checkUses(StmtRef, const VarRef&);
	bool checkUses(const ProcRef&, const VarRef&);
	StmtInfoPtrSet getStmtUsesByVar(const VarRef&);
	ProcRefSet getProcUsesByVar(const VarRef&);
	VarRefSet getUsesByStmt(StmtRef);
	VarRefSet getUsesByProc(const ProcRef&);

	// Modify get methods
	bool checkModifies(StmtRef, const VarRef&);
	bool checkModifies(const ProcRef&, const VarRef&);
	StmtInfoPtrSet getStmtModifiesByVar(const VarRef&);
	ProcRefSet getProcModifiesByVar(const VarRef&);
	VarRefSet getModifiesByStmt(StmtRef);
	VarRefSet getModifiesByProc(const ProcRef&);

	// Assign get methods
	bool patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& exp, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& exp, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& variable);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& exp,
	                                                                  bool is_exact_match);

	// Call get methods
	bool checkCall(const ProcRef& caller, const ProcRef& callee);
	ProcRefSet getCallee(const ProcRef& caller);
	ProcRefSet getCaller(const ProcRef& callee);

	// Others
	void clear();
	void populateComplexRelations();

	// For testing
	unordered_map<StmtRef, shared_ptr<StmtInfo>> getStmtInfoMap();

private:
	ConstantStore constant_store;
	VariableStore variable_store;
	StatementStore statement_store;
	StatementRelationStore<ParentRelation> parent_store;
	StatementRelationStore<FollowsRelation> follows_store;
	SVRelationStore<UsesRelation> uses_store;
	SVRelationStore<ModifiesRelation> modifies_store;
	AssignStore assign_store;
};

#endif
