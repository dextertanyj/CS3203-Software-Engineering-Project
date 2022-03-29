#ifndef SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::UsesDispatcher {
Types::ExecutorSetBundle dispatcher(const vector<Types::ReferenceArgument>& arguments);
}

#endif  // SPA_SRC_QP_DISPATCHER_USESDISPATCHER_H
