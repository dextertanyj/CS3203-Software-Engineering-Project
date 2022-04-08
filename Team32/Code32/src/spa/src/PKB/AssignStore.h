#ifndef SPA_SRC_PKB_ASSIGNSTORE_H
#define SPA_SRC_PKB_ASSIGNSTORE_H

#include <unordered_map>
#include <unordered_set>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/Hash.h"
#include "PKB/PKB.h"

class PKB::AssignStore {
public:
	AssignStore();
	void setAssign(const StmtInfoPtr& statement, const VarRef& variable, const Common::EP::Expression& expression);
	bool patternExists(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& var_name);
	StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::EP::Expression& expression, bool is_exact_match);
	std::unordered_map<VarRef, std::unordered_set<AssignRelation>> getAssignMap();
	void clear();

private:
	static bool compareExpressions(const Common::EP::Expression& expression, const Common::EP::Expression& op_tree, bool is_exact_match);
	std::unordered_map<VarRef, std::unordered_set<AssignRelation>> var_to_relation_store;
	std::unordered_map<Common::EP::Expression, std::unordered_set<AssignRelation>> exp_to_relation_store;
};

#endif
