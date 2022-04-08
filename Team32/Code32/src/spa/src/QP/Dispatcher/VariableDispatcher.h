#ifndef SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_H

#include <unordered_map>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::VariableDispatcher {

using namespace QP::Types;

template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getNameMap();
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getIndexMap();
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getProcedureMap();
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getStatementMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_H
