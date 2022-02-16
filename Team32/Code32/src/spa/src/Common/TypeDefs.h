#ifndef INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H
#define INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

using namespace std;

typedef int StmtRef;
typedef string VarRef;
typedef string ProcRef;

enum class StmtType { Assign, Print, Call, Read, WhileStmt, IfStmt };

typedef struct StmtInfo {
    StmtRef reference;
    StmtType type;
} StmtInfo;

typedef vector<shared_ptr<StmtInfo>> StmtInfoList;
typedef vector<VarRef> VarRefList;
typedef vector<StmtRef> StmtRefList;

typedef unordered_set<shared_ptr<StmtInfo>> StmtInfoPtrSet;
typedef unordered_set<VarRef> VarRefSet;
typedef unordered_set<StmtRef> StmtRefSet;

enum class MathematicalOperator { Plus, Minus, Times, Divide, Modulo, EQ, NEQ, LT, LTE, GT, GTE, And, Or, Not };

class Modifies;
class FollowsPKB;
class ParentPKB;
class Uses;

#endif


