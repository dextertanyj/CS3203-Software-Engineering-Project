#ifndef SPA_SRC_PKB_PKB_H
#define SPA_SRC_PKB_PKB_H

#include <memory>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"

namespace PKB {
class StorageUpdateInterface;
class StorageAccessInterface;
class Storage;

class StatementInfo;
class ProcedureInfo;

template <typename TIdent, typename TContent, class TInfo>
class InfoStore;

class AssignStore;
class ConstantStore;
template <class T>
class StatementRelationStore;
template <typename TIdent, class TInfo, class TRelation>
class TransitiveRelationStore;
class CallStatementStore;
template <class T>
class SVRelationStore;
class VariableStore;

template <class TInfo>
class TopologicalSort;

class CallRelation;
class FollowsRelation;
class ModifiesRelation;
class ParentRelation;
class UsesRelation;

typedef struct AssignRelation {
	shared_ptr<StmtInfo> node;
	VarRef variable;
	Common::ExpressionProcessor::Expression expression;
} AssignRelation;

namespace Types {};
}  // namespace PKB

#endif  // SPA_SRC_PKB_PKB_H
