#ifndef SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H

#include <unordered_map>
#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::FollowsNextDispatcher {

using namespace QP::Types;

template <QP::Types::ClauseType T>
ExecutorSetBundle dispatcher(const vector<ReferenceArgument>& arguments);
template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ReferenceArgument>& args);
template <ClauseType T>
const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>& getArgumentDispatchMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
