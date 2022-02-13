#ifndef INC_21S2_CP_SPA_TEAM_32_ASSIGNSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_ASSIGNSTORE_H

#include "../Common/TypeDefs.h"
#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include <unordered_map>
#include <memory>
#include "iostream"

using namespace std;

typedef struct {
    VarRef variableLHS;
    // Note: Use OperationTree instead of string when OperationTree is created
    Common::ArithmeticProcessor::ArithmeticExpression opTree;
} AssignRelation ;


class AssignStore {
public:
    AssignStore();
    void setAssign(StmtRef stmtNo, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree);

    bool patternExists(const VarRef &varName, const Common::ArithmeticProcessor::ArithmeticExpression &opTree, bool isRHSExactMatchNeeded);

    /**
     * Matches LHS variable and RHS operation tree only for specific statements given. Used for multi-clause
     * queries where statement list is already filtered through first clause.
     * Specify LHS variable as "" if matching of LHS variable is not required.
     */
    StmtRefList getPatternMatch(const StmtInfoList&, const VarRef&, const Common::ArithmeticProcessor::ArithmeticExpression&, bool isRHSExactMatchNeeded);

    /**
     * Matches LHS variable and RHS operation for all assign statements. Used for single pattern clause queries.
     */
    StmtRefList getStmtsWithPattern(const VarRef &variableLHS, const Common::ArithmeticProcessor::ArithmeticExpression &opTree, bool isRHSExactMatchNeeded);
    StmtRefList getStmtsWithPatternLHS(const VarRef &varName);
    vector<pair<StmtRef, VarRef>> getStmtsWithPatternRHS(const Common::ArithmeticProcessor::ArithmeticExpression &opTree, bool isRHSExactMatchNeeded);
    void clear();
private:
    bool compareOpTreeAndVar(AssignRelation assignRelation, const VarRef& variableLHS, const Common::ArithmeticProcessor::ArithmeticExpression& opTree, bool isRHSExactMatchNeeded);
    unordered_map<StmtRef, AssignRelation> assignMap;
};

#endif
