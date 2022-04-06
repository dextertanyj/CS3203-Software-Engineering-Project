#ifndef SPA_SRC_COMMON_TYPEDEFS_H
#define SPA_SRC_COMMON_TYPEDEFS_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "Common/Hash.h"

template <class... Ts>
struct Visitor : Ts... {
	using Ts::operator()...;
};

template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

using StmtRef = size_t;
using ConstVal = unsigned long long int;
using VarRef = std::string;
using ProcRef = std::string;

enum class StmtType { Assign, Print, Call, Read, While, If };
enum class MathematicalOperator { Plus, Minus, Times, Divide, Modulo, EQ, NEQ, LT, LTE, GT, GTE, And, Or, Not };

class StatementInfoInterface {
public:
	[[nodiscard]] virtual StmtRef getIdentifier() const = 0;
	[[nodiscard]] virtual StmtType getType() const = 0;
	virtual ~StatementInfoInterface() = default;
};

using StmtInfo = StatementInfoInterface;
using StmtInfoPtr = std::shared_ptr<StmtInfo>;
using StmtInfoPtrSet = std::unordered_set<StmtInfoPtr>;
using VarRefSet = std::unordered_set<VarRef>;
using ConstValSet = std::unordered_set<ConstVal>;
using ProcRefSet = std::unordered_set<ProcRef>;
using StmtRefSet = std::unordered_set<StmtRef>;
using StmtInfoPtrVarRefSet = std::unordered_set<std::pair<StmtInfoPtr, VarRef>>;

struct LessComparator {
	bool operator()(const StmtInfoPtr& lhs, const StmtInfoPtr& rhs) const { return lhs->getIdentifier() < rhs->getIdentifier(); }
};

struct GreaterComparator {
	bool operator()(const StmtInfoPtr& lhs, const StmtInfoPtr& rhs) const { return lhs->getIdentifier() > rhs->getIdentifier(); }
};

#endif
