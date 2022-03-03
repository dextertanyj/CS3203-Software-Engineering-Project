#ifndef SPA_SRC_PKB_STORAGE_H
#define SPA_SRC_PKB_STORAGE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/AssignStore.h"
#include "PKB/CallRelation.h"
#include "PKB/CallStatementStore.h"
#include "PKB/FollowsRelation.h"
#include "PKB/InfoStore.h"
#include "PKB/ModifiesPRelation.h"
#include "PKB/ModifiesSRelation.h"
#include "PKB/UsesPRelation.h"
#include "PKB/UsesSRelation.h"
#include "PKB/PKB.h"
#include "PKB/PVRelationStore.tpp"
#include "PKB/ParentRelation.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/SetStore.tpp"
#include "PKB/StatementInfo.h"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StorageAccessInterface.h"
#include "PKB/StorageUpdateInterface.h"
#include "PKB/TopologicalSort.tpp"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/VariableStore.h"

using namespace std;

class PKB::Storage : public PKB::StorageUpdateInterface, public PKB::StorageAccessInterface {
public:
	// Set methods called by Source processor
	void setProc(ProcRef procedure, StmtRef start, StmtRef end) override;
	void setCall(StmtRef index, ProcRef name) override;
	void setFollows(StmtRef front, StmtRef back) override;
	void setParent(StmtRef parent, StmtRef child) override;
	void setStmtType(StmtRef index, StmtType type) override;
	void setConstant(ConstVal value) override;
	void setConstant(const unordered_set<ConstVal>& values) override;
	void setUses(StmtRef index, VarRef name) override;
	void setModifies(StmtRef index, VarRef name) override;
	void setUses(StmtRef index, VarRefSet names) override;
	void setModifies(StmtRef index, VarRefSet names) override;
	void setAssign(StmtRef index, VarRef variable, Common::ExpressionProcessor::Expression expression) override;

	// Get methods called by PQL

	// General get methods
	StmtInfoPtrSet getStatements() override;
	VarRefSet getVariables() override;
	unordered_set<ConstVal> getConstants() override;
	unordered_set<ProcRef> getProcedures() override;

	// Parent get methods
	shared_ptr<StmtInfo> getParent(StmtRef index) override;
	StmtInfoPtrSet getChildren(StmtRef index) override;
	bool checkParents(StmtRef parent, StmtRef child) override;
	StmtInfoPtrSet getParentStar(StmtRef index) override;
	StmtInfoPtrSet getChildStar(StmtRef index) override;

	// Follow get methods
	shared_ptr<StmtInfo> getPreceding(StmtRef index) override;
	shared_ptr<StmtInfo> getFollower(StmtRef index) override;
	bool checkFollows(StmtRef front, StmtRef back) override;
	StmtInfoPtrSet getFollowerStar(StmtRef index) override;
	StmtInfoPtrSet getPrecedingStar(StmtRef index) override;

	// Use get methods
	bool checkUses(StmtRef index, const VarRef& name) override;
	bool checkUses(const ProcRef& procedure_name, const VarRef& variable_name) override;
	StmtInfoPtrSet getStmtUsesByVar(const VarRef& name) override;
	ProcRefSet getProcUsesByVar(const VarRef& name) override;
	VarRefSet getUsesByStmt(StmtRef index) override;
	VarRefSet getUsesByProc(const ProcRef& name) override;

	// Modify get methods
	bool checkModifies(StmtRef index, const VarRef& name) override;
	bool checkModifies(const ProcRef& procedure_name, const VarRef& variable_name) override;
	StmtInfoPtrSet getStmtModifiesByVar(const VarRef& name) override;
	ProcRefSet getProcModifiesByVar(const VarRef& name) override;
	VarRefSet getModifiesByStmt(StmtRef index) override;
	VarRefSet getModifiesByProc(const ProcRef& name) override;

	// Assign get methods
	bool patternExists(const VarRef& name, const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) override;
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& name, const Common::ExpressionProcessor::Expression& expression,
	                                   bool is_exact_match) override;
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& name) override;
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
	                                                                  bool is_exact_match) override;

	// Call get methods
	bool checkCall(const ProcRef& caller, const ProcRef& callee) override;
	ProcRefSet getCallee(const ProcRef& caller) override;
	ProcRefSet getCalleeStar(const ProcRef& caller) override;
	ProcRefSet getCaller(const ProcRef& callee) override;
	ProcRefSet getCallerStar(const ProcRef& callee) override;

	// Others
	void populateComplexRelations() override;
	void clear();

	// For testing
	unordered_map<StmtRef, shared_ptr<StmtInfo>> getStmtInfoMap();

private:
	SetStore<ConstVal> constant_store;
	SetStore<VarRef> variable_store;
	Types::StatementStore statement_store;
	Types::ProcedureStore procedure_store;
	CallStatementStore call_statement_store;
	TopologicalSort<ProcedureInfo> call_graph;
	TransitiveRelationStore<ProcRef, ProcedureInfo, CallRelation> call_store;
	StatementRelationStore<ParentRelation> parent_store;
	StatementRelationStore<FollowsRelation> follows_store;
	SVRelationStore<UsesSRelation> uses_s_store;
	SVRelationStore<ModifiesSRelation> modifies_s_store;
	PVRelationStore<UsesPRelation> uses_p_store;
	PVRelationStore<PKB::ModifiesPRelation> modifies_p_store;
	AssignStore assign_store;

	static ProcRefSet procedureInfoToProcRef(const unordered_set<shared_ptr<ProcedureInfo>>& set);
	static StmtInfoPtrSet statementInfoPtrSetToInterfacePtrSet(const unordered_set<shared_ptr<StatementInfo>>& set);
};

#endif
