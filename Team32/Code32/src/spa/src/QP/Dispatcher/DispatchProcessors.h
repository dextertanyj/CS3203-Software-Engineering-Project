#ifndef SPA_SRC_QP_DISPATCHPROCESSORS_H
#define SPA_SRC_QP_DISPATCHPROCESSORS_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::DispatchProcessors {
template <typename T>
Types::ExecutorSetBundle processArgument(Types::ClauseType type, T map, const vector<Types::ReferenceArgument>& args);

template <typename T>
Types::ExecutorSetBundle processArgument(T map, const vector<Types::ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_DISPATCHPROCESSORS_H
