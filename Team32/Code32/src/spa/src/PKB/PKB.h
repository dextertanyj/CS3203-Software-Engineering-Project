#ifndef SPA_SRC_PKB_PKB_H
#define SPA_SRC_PKB_PKB_H

#include <memory>
#include <stdexcept>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/Hash.h"
#include "Common/TypeDefs.h"

namespace PKB {
class StorageUpdateInterface;
class StorageAccessInterface;
class Storage;

class StatementInfo;
class ProcedureInfo;

template <typename TIdent, typename TContent, class TInfo>
class InfoStore;
template <typename T>
class SetStore;
template <typename TIdent, class TInfo, class TRelation>
class TransitiveRelationStore;
class CallsStatementStore;
template <class T>
class SVRelationStore;
template <class T>
class PVRelationStore;
template <class TInfo>
class TopologicalSort;
template <typename T>
class TransitiveRelation;

class OrderedStatementTransitiveRelation;

class AssignStore;
class CallsStatementStore;
class CallsRelation;
class FollowsRelation;
class ModifiesSRelation;
class ModifiesPRelation;
class ParentRelation;
class UsesSRelation;
class UsesPRelation;
class NextManager;
class AffectsManager;

struct IfControlRelation {
	static bool validate(SVRelationStore<IfControlRelation>* /*store*/, const StmtInfoPtr& statement, const VarRef& /*var*/) {
		return statement->getType() == StmtType::If;
	}
	static bool validate(SVRelationStore<IfControlRelation>* /*store*/, const StmtInfoPtr& statement, const VarRefSet& /*var_set*/) {
		return statement->getType() == StmtType::If;
	}
};

struct WhileControlRelation {
	static bool validate(SVRelationStore<WhileControlRelation>* /*store*/, const StmtInfoPtr& statement, const VarRef& /*var*/) {
		return statement->getType() == StmtType::While;
	}
	static bool validate(SVRelationStore<WhileControlRelation>* /*store*/, const StmtInfoPtr& statement, const VarRefSet& /*var_set*/) {
		return statement->getType() == StmtType::While;
	}
};

struct AssignRelation {
	StmtInfoPtr node;                   // NOLINT(misc-non-private-member-variables-in-classes)
	VarRef variable;                    // NOLINT(misc-non-private-member-variables-in-classes)
	Common::EP::Expression expression;  // NOLINT(misc-non-private-member-variables-in-classes)
	inline bool operator==(const AssignRelation& other) const {
		return node == other.node && variable == other.variable && expression == other.expression;
	}

	friend std::hash<AssignRelation>;
};

struct DuplicateEntityException : public std::runtime_error {
	using runtime_error::runtime_error;
};

struct CallGraphException : public std::runtime_error {
	using runtime_error::runtime_error;
};

struct TopologicalSortException : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

namespace CFG {}
namespace Types {}
}

#endif  // SPA_SRC_PKB_PKB_H
