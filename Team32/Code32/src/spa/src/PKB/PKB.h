#ifndef SPA_SRC_PKB_PKB_H
#define SPA_SRC_PKB_PKB_H

#include <memory>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"

namespace PKB {
class StorageUpdateInterface;
class StorageAccessInterface;
class Storage;

class StatementInfo;
class ProcedureInfo;
class NodeInfo;
class DummyNodeInfo;

template <typename TIdent, typename TContent, class TInfo>
class InfoStore;
template <typename T>
class SetStore;
template <typename TIdent, class TInfo, class TRelation>
class TransitiveRelationStore;
class CallStatementStore;
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
class CallStatementStore;
class CallRelation;
class FollowsRelation;
class ModifiesSRelation;
class ModifiesPRelation;
class ParentRelation;
class UsesSRelation;
class UsesPRelation;
class NodeRelation;
class NextStarQueryCache;
class NodeInterface;
class IfNode;
class WhileNode;
class DummyNode;
class NonConditionalNode;
class ControlFlowGraph;

typedef struct IfControlRelation {
	static bool validate(SVRelationStore<IfControlRelation>*, const shared_ptr<StmtInfo>& statement, const VarRef&) {
		return statement->getType() == StmtType::IfStmt;
	}
	static bool validate(SVRelationStore<IfControlRelation>*, const shared_ptr<StmtInfo>& statement, const VarRefSet&) {
		return statement->getType() == StmtType::IfStmt;
	}
} IfControlRelation;

typedef struct WhileControlRelation {
	static bool validate(SVRelationStore<WhileControlRelation>*, const shared_ptr<StmtInfo>& statement, const VarRef&) {
		return statement->getType() == StmtType::WhileStmt;
	}
	static bool validate(SVRelationStore<WhileControlRelation>*, const shared_ptr<StmtInfo>& statement, const VarRefSet&) {
		return statement->getType() == StmtType::WhileStmt;
	}
} WhileControlRelation;

typedef struct AssignRelation {
	shared_ptr<StmtInfo> node;
	VarRef variable;
	Common::ExpressionProcessor::Expression expression;
} AssignRelation;

namespace Types {};
}  // namespace PKB

#endif  // SPA_SRC_PKB_PKB_H
