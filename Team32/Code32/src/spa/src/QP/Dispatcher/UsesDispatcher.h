#ifndef SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/Types.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::UsesDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;
}

#endif  // SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H
