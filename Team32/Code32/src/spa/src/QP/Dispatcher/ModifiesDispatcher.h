#ifndef SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H

#include <vector>

#include "QP/ClauseArgument.h"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::ModifiesDispatcher {

Types::ExecutorSetBundle dispatcher(const std::vector<ClauseArgument>& args);
}

#endif  // SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H
