#ifndef SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_H

#include "QP/Types.h"

namespace QP::Dispatcher::VariableDispatcher {

using namespace std;
using namespace QP::Types;

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getNameMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getIndexMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getProcedureMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getStatementMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_H
