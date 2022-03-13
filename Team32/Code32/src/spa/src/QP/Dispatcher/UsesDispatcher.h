#ifndef SPA_SRC_QP_RELATIONSHIP_USES_H
#define SPA_SRC_QP_RELATIONSHIP_USES_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::UsesDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;
}  // namespace QP::Dispatcher::UsesDispatcher

#endif  // SPA_SRC_QP_RELATIONSHIP_USES_H
