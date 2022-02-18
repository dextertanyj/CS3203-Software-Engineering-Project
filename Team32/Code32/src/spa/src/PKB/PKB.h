#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/AssignStore.h"
#include "PKB/ConstantStore.h"
#include "PKB/FollowsRelation.h"
#include "PKB/Modifies.h"
#include "PKB/ParentRelation.h"
#include "PKB/SVRelationStore.tpp"
#include "PKB/StatementRelationStore.tpp"
#include "PKB/StatementStore.h"
#include "PKB/VariableStore.h"

using namespace std;

class PKB {
public:
	PKB();

	// Set methods called by Source processor
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
	StmtInfoPtrSet getUsesByVar(const VarRef&);
	VarRefSet getUsesByStmt(StmtRef);

	// Modify get methods
	bool checkModifies(StmtRef, const VarRef&);
	StmtInfoPtrSet getModifiesByVar(const VarRef&);
	VarRefSet getModifiesByStmt(StmtRef);

	// Assign get methods
	bool patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& e, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& e, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& variable);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& e,
	                                                                  bool is_exact_match);

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
	SVRelationStore<Uses> uses_store;
	SVRelationStore<Modifies> modifies_store;
	AssignStore assign_store;
};
