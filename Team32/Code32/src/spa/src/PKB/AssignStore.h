#ifndef SPA_SRC_PKB_ASSIGNSTORE_H
#define SPA_SRC_PKB_ASSIGNSTORE_H

#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

using namespace std;

class PKB::AssignStore {
public:
	AssignStore();
	void setAssign(const shared_ptr<StmtInfo>& statement, const VarRef& variable, const Common::EP::Expression& expression);
	bool patternExists(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& var_name);
	StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::EP::Expression& expression, bool is_exact_match);
	unordered_map<VarRef, unordered_set<PKB::AssignRelation, PKB::AssignRelation::Hasher>> getAssignMap();
	void clear();

private:
	static bool compareExpressions(const Common::EP::Expression& expression, const Common::EP::Expression& op_tree, bool is_exact_match);
	unordered_map<VarRef, unordered_set<PKB::AssignRelation, PKB::AssignRelation::Hasher>> var_to_relation_store;
	unordered_map<Common::EP::Expression, unordered_set<PKB::AssignRelation, PKB::AssignRelation::Hasher>> exp_to_relation_store;
};

#endif
