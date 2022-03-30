#ifndef SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::FollowsNextDispatcher {

using namespace QP::Types;

template <QP::Types::ClauseType T>
Types::ExecutorSetBundle dispatcher(const vector<Types::ReferenceArgument>& arguments);

template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ReferenceArgument>& args);

template <ClauseType T>
unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> getArgumentDispatchMap();

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getIndexMap();

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getWildcardMap();

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getSynonymMap();

}

#endif  // SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
