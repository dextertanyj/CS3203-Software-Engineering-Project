#ifndef SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H

#include <unordered_map>
#include <utility>

#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::ModifiesDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;
};

#endif  // SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H
