#ifndef SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_H

#include <unordered_map>
#include <vector>

#include "QP/ClauseArgument.h"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::AffectsDispatcher {

using namespace QP::Types;

template <ClauseType T>
ExecutorSetBundle dispatcher(const std::vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const std::vector<ClauseArgument>& args);
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, std::unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>& getArgumentDispatchMap();
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getIndexMap();
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap();
template <ClauseType T>
const std::unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap();
}

#endif  // SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_H
