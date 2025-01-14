#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H

#include <unordered_map>
#include <vector>

#include "QP/ClauseArgument.h"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::CallsDispatcher {

using namespace std;
using namespace QP::Types;

template <ClauseType T>
ExecutorSetBundle dispatcher(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ClauseArgument>& args);
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>& getArgumentDispatchMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getNameMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H
