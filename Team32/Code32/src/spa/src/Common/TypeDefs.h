#ifndef SPA_SRC_COMMON_TYPEDEFS_H
#define SPA_SRC_COMMON_TYPEDEFS_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "Common/Hash.h"

using namespace std;

template <class... Ts>
struct Visitor : Ts... {
	using Ts::operator()...;
};

template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

typedef size_t StmtRef;
typedef unsigned long long int ConstVal;
typedef string VarRef;
typedef string ProcRef;

enum class StmtType { Assign, Print, Call, Read, WhileStmt, IfStmt };

class StatementInfoInterface {
public:
	[[nodiscard]] virtual StmtRef getIdentifier() const = 0;
	[[nodiscard]] virtual StmtType getType() const = 0;
	virtual ~StatementInfoInterface() = default;
};

typedef StatementInfoInterface StmtInfo;

typedef unordered_set<shared_ptr<StmtInfo>> StmtInfoPtrSet;
typedef unordered_set<VarRef> VarRefSet;
typedef unordered_set<ProcRef> ProcRefSet;
typedef unordered_set<StmtRef> StmtRefSet;
typedef unordered_set<pair<shared_ptr<StmtInfo>, VarRef>, Common::Hash::PairHash> StmtInfoPtrVarRefSet;

enum class MathematicalOperator { Plus, Minus, Times, Divide, Modulo, EQ, NEQ, LT, LTE, GT, GTE, And, Or, Not };

#endif
