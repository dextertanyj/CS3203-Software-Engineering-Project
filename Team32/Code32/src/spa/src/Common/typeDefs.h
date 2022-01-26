//
// Created by Yanzieee on 26/1/2022.
//

#ifndef INC_21S2_CP_SPA_TEAM_32_UTIL_H
#define INC_21S2_CP_SPA_TEAM_32_UTIL_H

#endif //INC_21S2_CP_SPA_TEAM_32_UTIL_H

#include <string>

using namespace std;

typedef int StmtRef;
typedef std::string VarRef;
typedef std::string ProcRef;

enum StmtType { assign, print, call, read, whileStmt, ifStmt, standard };

typedef struct StmtInfo {
    StmtRef reference;
    StmtType type;
}

typedef std::vector<StmtInfo> StmtInfoList;
typedef std::vector<VarRef> VarRefList;

typedef struct Synonym {
    QueryType type;
    std::string symbol;
} Synonym;

typedef struct SuchThatClause {
    Relationship relation;
    Pair<Synonym, Pair<StmtRef, VarRef> lhs;
} SuchThatClause;

typedef struct PatternClause {
    Synonym lhs;
    // Some kind of math expression?
} PatternClause;



