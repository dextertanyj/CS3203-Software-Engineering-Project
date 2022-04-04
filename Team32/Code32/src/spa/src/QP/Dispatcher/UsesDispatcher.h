#ifndef SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H

#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::UsesDispatcher {

using namespace QP::Types;

ExecutorSetBundle dispatcher(const std::vector<ReferenceArgument>& args);
}

#endif  // SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H
