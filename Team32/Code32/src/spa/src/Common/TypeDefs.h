#ifndef INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H
#define INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H

using namespace std;

typedef int StmtRef;
typedef string VarRef;
typedef string ProcRef;

enum class StmtType { Assign, Print, Call, Read, WhileStmt, IfStmt, Unknown };

typedef struct StmtInfo {
    StmtRef reference;
    StmtType type;
} StmtInfo;

typedef vector<StmtInfo> StmtInfoList;
typedef vector<VarRef> VarRefList;

#endif INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H


