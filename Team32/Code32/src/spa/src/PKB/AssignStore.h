#ifndef INC_21S2_CP_SPA_TEAM_32_ASSIGNSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_ASSIGNSTORE_H

#include <memory>
#include <unordered_map>

#include "../Common/TypeDefs.h"
#include "Common/ExpressionProcessor/Expression.h"
#include "iostream"

using namespace std;

typedef struct AssignRelation {
	shared_ptr<StmtInfo> node;
	VarRef variable;
	Common::ExpressionProcessor::Expression expression;
} AssignRelation;

class AssignStore {
public:
	AssignStore();
	void setAssign(shared_ptr<StmtInfo> statement, VarRef variable, Common::ExpressionProcessor::Expression expression);
	bool patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression,
	                                   bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& varName);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
	                                                                         bool is_exact_match);
	unordered_map<StmtRef, AssignRelation> getAssignMap();
	void clear();

private:
	static bool compareExpressions(AssignRelation& relation, const VarRef& variable, const Common::ExpressionProcessor::Expression& opTree,
	                               bool is_exact_match);
	unordered_map<StmtRef, AssignRelation> store;
};

#endif
