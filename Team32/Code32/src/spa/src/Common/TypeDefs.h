#ifndef INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H
#define INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H

#include <string>
#include <vector>

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
typedef vector<StmtRef> StmtRefList;

enum class MathematicalOperator { Plus, Minus, Times, Divide, Modulo, EQ, NEQ, LT, LTE, GT, GTE, And, Or, Not };

#endif


