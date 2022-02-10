#ifndef INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H
#define INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H

#include <string>
#include <vector>
#include <memory>

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
typedef vector<shared_ptr<StmtInfo>> StmtInfoPtrList;
typedef vector<VarRef> VarRefList;
typedef vector<StmtRef> StmtRefList;

enum class ArithmeticOperator { Plus, Minus, Times, Divide, Modulo };
enum class RelationalOperator { EQ, NEQ, LT, LTE, GT, GTE };

#endif


