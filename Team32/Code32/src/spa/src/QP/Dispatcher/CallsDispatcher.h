#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::CallsDispatcher {

using namespace QP::Types;

template <ClauseType T>
ExecutorSetBundle dispatcher(const vector<ReferenceArgument>& arguments);

template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ReferenceArgument>& args);

template <ClauseType T>
unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> getArgumentDispatchMap();

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getNameMap();

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getWildcardMap();

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getSynonymMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H
