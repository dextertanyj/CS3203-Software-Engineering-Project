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

class AssignStore;
class ConstantStore;
class ProcedureStore;
template <class T>
class StatementRelationStore;
class StatementStore;
template <class T>
class SVRelationStore;
class VariableStore;

class FollowsRelation;
class ModifiesRelation;
class ParentRelation;
class UsesRelation;

typedef struct AssignRelation {
	shared_ptr<StmtInfo> node;
	VarRef variable;
	Common::ExpressionProcessor::Expression expression;
} AssignRelation;

typedef struct ProcRelation {
	vector<shared_ptr<StmtInfo>> idx_list;
	shared_ptr<StmtInfo> call_stmt;
} ProcRelation;
}  // namespace PKB

#endif  // SPA_SRC_PKB_PKB_H
