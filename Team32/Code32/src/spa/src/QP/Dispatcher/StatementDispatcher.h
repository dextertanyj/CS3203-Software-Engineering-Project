#ifndef SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_H

#include <unordered_map>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::StatementDispatcher {

using namespace std;
using namespace QP::Types;

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getIndexMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap();
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_H
