#ifndef SPA_SRC_DISPATCHER_WITHDISPATCHER_H
#define SPA_SRC_DISPATCHER_WITHDISPATCHER_H

#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ClauseArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::WithDispatcher {

using namespace QP::Types;

ExecutorSetBundle dispatcher(const std::vector<ClauseArgument>& args);
}

#endif  // SPA_SRC_DISPATCHER_WITHDISPATCHER_H
