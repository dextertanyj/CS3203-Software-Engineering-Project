#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H

#include "QP/ClauseArgument.h"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/QP.h"
#include "QP/Types.h"

namespace QP::Dispatcher::DispatchProcessors {
template <typename T>
Types::ExecutorSetBundle processArgument(Types::ClauseType type, const T& map, const std::vector<ClauseArgument>& args);

template <typename T>
Types::ExecutorSetBundle processArgument(const T& map, const std::vector<ClauseArgument>& args);
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H
