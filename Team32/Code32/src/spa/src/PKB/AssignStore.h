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
    StmtRefList getStmtsWithPattern(const VarRef &variableLHS, const Common::ArithmeticProcessor::ArithmeticExpression &opTree, bool isRHSExactMatchNeeded);
    StmtRefList getStmtsWithPatternLHS(const VarRef &varName);
    vector<pair<StmtRef, VarRef>> getStmtsWithPatternRHS(const Common::ArithmeticProcessor::ArithmeticExpression &opTree, bool isRHSExactMatchNeeded);
    unordered_map<StmtRef, AssignRelation> getAssignMap();
    void clear();
private:
    static bool compareOpTreeAndVar(AssignRelation assignRelation, const VarRef& variableLHS, const Common::ArithmeticProcessor::ArithmeticExpression& opTree, bool isRHSExactMatchNeeded);
    unordered_map<StmtRef, AssignRelation> assignMap;
};

#endif
