#ifndef INC_21S2_CP_SPA_TEAM_32_ASSIGNSTORE_H
#define INC_21S2_CP_SPA_TEAM_32_ASSIGNSTORE_H

#include "../Common/TypeDefs.h"
#include <unordered_map>

using namespace std;

typedef struct {
    VarRef variableLHS;
    // Note: Use OperationTree instead of string when OperationTree is created
    string opTree;
} AssignRelation ;

class AssignStore {
public:
    AssignStore();
    void setAssign(StmtRef stmtNo, VarRef variableLHS, string opTree);

    /**
     * Matches LHS variable and RHS operation tree only for specific statements given. Used for multi-clause
     * queries where statement list is already filtered through first clause.
     * Specify LHS variable as "" if matching of LHS variable is not required.
     */
    StmtRefList getPatternMatch(StmtInfoList stmtNoList, VarRef variableLHS, string opTree, bool isRHSExactMatchNeeded);

    /**
     * Matches LHS variable and RHS operation for all assign statements. Used for single pattern clause queries.
     */
    StmtRefList getAllPatternMatch(VarRef variableLHS, string opTree, bool isRHSExactMatchNeeded);
private:
    bool compareOpTreeAndVar(AssignRelation assignRelation, VarRef variableLHS, string opTree, bool isRHSExactMatchNeeded);
    unordered_map<StmtRef, AssignRelation> assignMap;
};

#endif
