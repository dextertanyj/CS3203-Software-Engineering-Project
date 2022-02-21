#ifndef INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H
#define INC_21S2_CP_SPA_TEAM_32_TYPEDEFS_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

typedef size_t StmtRef;
typedef unsigned long long int ConstVal;
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

#endif
