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

    template<typename TIdent, typename TContent, class TInfo>
    class InfoStore;

    template<typename T>
    class SetStore;

    template<typename TIdent, class TInfo, class TRelation>
    class TransitiveRelationStore;

    class CallsStatementStore;

    template<class T>
    class SVRelationStore;

    template<class T>
    class PVRelationStore;

    template<class TInfo>
    class TopologicalSort;

    template<typename T>
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

    class NodeInterface;

    class IfNode;

    class WhileNode;

    class DummyNode;

    class NonConditionalNode;

    class StatementNode;

    class ControlFlowGraph;

    class NextManager;

    class AffectsManager;

    typedef struct IfControlRelation {
        static bool validate(SVRelationStore<IfControlRelation> * /*store*/, const shared_ptr<StmtInfo> &statement,
                             const VarRef & /*var*/) {
            return statement->getType() == StmtType::IfStmt;
        }

        static bool validate(SVRelationStore<IfControlRelation> * /*store*/, const shared_ptr<StmtInfo> &statement,
                             const VarRefSet & /*var_set*/) {
            return statement->getType() == StmtType::IfStmt;
        }
    } IfControlRelation;

    typedef struct WhileControlRelation {
        static bool validate(SVRelationStore<WhileControlRelation> * /*store*/, const shared_ptr<StmtInfo> &statement,
                             const VarRef & /*var*/) {
            return statement->getType() == StmtType::WhileStmt;
        }

        static bool validate(SVRelationStore<WhileControlRelation> * /*store*/, const shared_ptr<StmtInfo> &statement,
                             const VarRefSet & /*var_set*/) {
            return statement->getType() == StmtType::WhileStmt;
        }
    } WhileControlRelation;

    typedef struct AssignRelation {
        shared_ptr<StmtInfo> node;
        VarRef variable;
        Common::ExpressionProcessor::Expression expression;
    } AssignRelation;

    enum class NodeType {
        Dummy, If, While, NonConditional
    };

    namespace Types {};
}

#endif  // SPA_SRC_PKB_PKB_H
