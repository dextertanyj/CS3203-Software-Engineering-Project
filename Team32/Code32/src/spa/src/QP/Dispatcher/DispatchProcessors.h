#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/QP.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::DispatchProcessors {
template <typename T>
Types::ExecutorSetBundle processArgument(Types::ClauseType type, T map, const std::vector<ReferenceArgument>& args);

template <typename T>
Types::ExecutorSetBundle processArgument(T map, const std::vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_H
