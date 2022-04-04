#ifndef SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_H

#include <unordered_map>
#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::ParentDispatcher {

using namespace QP::Types;

template <ClauseType T>
ExecutorSetBundle dispatcher(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const std::vector<ReferenceArgument>& args);
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, std::unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>& getArgumentDispatchMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_H
