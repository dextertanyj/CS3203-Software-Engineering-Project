#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/QP.h"
#include "QP/ClauseArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::DispatchProcessors {
template <typename T>
Types::ExecutorSetBundle processArgument(Types::ClauseType type, T map, const std::vector<ClauseArgument>& args);

template <typename T>
Types::ExecutorSetBundle processArgument(T map, const std::vector<ClauseArgument>& args);
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H
