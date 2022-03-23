#ifndef SPA_SRC_PKB_STORAGE_H
#define SPA_SRC_PKB_STORAGE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/AffectsManager.h"
#include "PKB/AssignStore.h"
#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/CallsRelation.h"
#include "PKB/CallsStatementStore.h"
#include "PKB/FollowsRelation.h"
#include "PKB/InfoStore.h"
#include "PKB/ModifiesPRelation.h"
#include "PKB/ModifiesSRelation.h"
#include "PKB/NextManager.h"
#include "PKB/PKB.h"
#include "PKB/PVRelationStore.tpp"
#include "PKB/ParentRelation.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/SetStore.tpp"
#include "PKB/StatementInfo.h"
#include "PKB/StorageAccessInterface.h"
#include "PKB/StorageUpdateInterface.h"
#include "PKB/TopologicalSort.tpp"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/UsesPRelation.h"
#include "PKB/UsesSRelation.h"

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
	void setIfControl(StmtRef index, VarRefSet names) override;
	void setIfControl(StmtRef index, VarRef name) override;
	void setWhileControl(StmtRef index, VarRefSet names) override;
	void setWhileControl(StmtRef index, VarRef name) override;
	void setNext(StmtRef previous, StmtRef next) override;
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) override;
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) override;

	// Get methods called by PQL

	// General get methods
	StmtInfoPtrSet getStatements() override;
	VarRefSet getVariables() override;
	unordered_set<ConstVal> getConstants() override;
	unordered_set<ProcRef> getProcedures() override;

	// Parent get methods
	bool checkParent(StmtRef parent, StmtRef child) override;
	bool checkParentStar(StmtRef parent, StmtRef child) override;
	shared_ptr<StmtInfo> getParent(StmtRef index) override;
	StmtInfoPtrSet getChildren(StmtRef index) override;
	StmtInfoPtrSet getParentStar(StmtRef index) override;
	StmtInfoPtrSet getChildStar(StmtRef index) override;

	// Follow get methods
	bool checkFollows(StmtRef front, StmtRef back) override;
	bool checkFollowsStar(StmtRef front, StmtRef back) override;
	shared_ptr<StmtInfo> getPreceding(StmtRef index) override;
	shared_ptr<StmtInfo> getFollower(StmtRef index) override;
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
	StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) override;

	// Call get methods
	bool checkCalls(const ProcRef& caller, const ProcRef& callee) override;
	bool checkCallsStar(const ProcRef& caller, const ProcRef& callee) override;
	ProcRefSet getCallee(const ProcRef& caller) override;
	ProcRefSet getCalleeStar(const ProcRef& caller) override;
	ProcRefSet getCaller(const ProcRef& callee) override;
	ProcRefSet getCallerStar(const ProcRef& callee) override;

	// Next get methods
	bool checkNext(StmtRef first, StmtRef second) override;
	bool checkNextStar(StmtRef first, StmtRef second) override;
	StmtInfoPtrSet getNext(StmtRef first) override;
	StmtInfoPtrSet getNextStar(StmtRef node_ref) override;
	StmtInfoPtrSet getPrevious(StmtRef second) override;
	StmtInfoPtrSet getPreviousStar(StmtRef node_ref) override;

	// Affects get methods
	bool checkAffects(StmtRef first, StmtRef second) override;
	bool checkAffectsStar(StmtRef first, StmtRef second) override;
	StmtInfoPtrSet getAffects(StmtRef first) override;
	StmtInfoPtrSet getAffectsStar(StmtRef first) override;
	StmtInfoPtrSet getAffected(StmtRef second) override;
	StmtInfoPtrSet getAffectedStar(StmtRef second) override;

	// Control Variable get methods
	bool checkIfControl(StmtRef index, VarRef name) override;
	bool checkWhileControl(StmtRef index, VarRef name) override;
	VarRefSet getIfControlVar(StmtRef index) override;
	VarRefSet getWhileControlVar(StmtRef index) override;
	StmtInfoPtrSet getIfControlStmt(VarRef name) override;
	StmtInfoPtrSet getWhileControlStmt(VarRef name) override;

	// Attribute get methods
	ProcRef getCalledProcedure(StmtRef index) override;

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
	CallsStatementStore call_statement_store;
	TopologicalSort<ProcedureInfo> call_graph;
	Types::CallStore call_store;
	Types::ParentStore parent_store;
	Types::FollowsStore follows_store;
	SVRelationStore<UsesSRelation> uses_s_store;
	SVRelationStore<ModifiesSRelation> modifies_s_store;
	PVRelationStore<UsesPRelation> uses_p_store;
	PVRelationStore<ModifiesPRelation> modifies_p_store;
	AssignStore assign_store;
	SVRelationStore<IfControlRelation> if_control_store;
	SVRelationStore<WhileControlRelation> while_control_store;
	ControlFlowGraph control_flow_graph;
	NextManager next_manager = NextManager(control_flow_graph);
	AffectsManager affects_manager = AffectsManager(control_flow_graph, modifies_s_store, uses_s_store);

	static ProcRefSet procedureInfoToProcRef(const unordered_set<shared_ptr<ProcedureInfo>>& set);
	static StmtInfoPtrSet statementInfoPtrSetToInterfacePtrSet(const unordered_set<shared_ptr<StatementInfo>>& set);
	void setNode(shared_ptr<StmtInfo> info);
};

#endif
