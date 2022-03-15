#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIES_H

#include <unordered_map>
#include <utility>

#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::ModifiesDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;
};  // namespace QP::Dispatcher::ModifiesDispatcher

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
