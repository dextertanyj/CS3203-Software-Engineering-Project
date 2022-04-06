#ifndef SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H

#include <vector>

#include "QP/ClauseArgument.h"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::PatternIfDispatcher {

using namespace QP::Types;

ExecutorSetBundle dispatcher(const std::vector<ClauseArgument>& args);
}

#endif  // SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H
