#ifndef SPA_SRC_DISPATCHER_WITHDISPATCHER_H
#define SPA_SRC_DISPATCHER_WITHDISPATCHER_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::WithDispatcher {
Types::ExecutorSetBundle dispatcher(const vector<Types::ReferenceArgument>& arguments);
}

#endif  // SPA_SRC_DISPATCHER_WITHDISPATCHER_H
