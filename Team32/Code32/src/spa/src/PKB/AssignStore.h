#ifndef SPA_SRC_PKB_ASSIGNSTORE_H
#define SPA_SRC_PKB_ASSIGNSTORE_H

#include <memory>
#include <unordered_map>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

using namespace std;

class PKB::AssignStore {
public:
	AssignStore();
	void setAssign(const shared_ptr<StmtInfo>& statement, VarRef variable, Common::ExpressionProcessor::Expression expression);
	bool patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression,
	                                   bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& var_name);
	StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
	                                                                  bool is_exact_match);
	unordered_map<StmtRef, PKB::AssignRelation> getAssignMap();
	void clear();

private:
	static bool compareExpressions(AssignRelation& relation, const VarRef& variable, const Common::ExpressionProcessor::Expression& op_tree,
	                               bool is_exact_match);
	unordered_map<StmtRef, PKB::AssignRelation> store;
};

#endif
