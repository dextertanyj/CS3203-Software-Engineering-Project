#ifndef SPA_SRC_DISPATCHER_WITHDISPATCHER_H
#define SPA_SRC_DISPATCHER_WITHDISPATCHER_H

#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::WithDispatcher {

using namespace QP::Types;

ExecutorSetBundle dispatcher(const vector<ReferenceArgument>& arguments);
}

#endif  // SPA_SRC_DISPATCHER_WITHDISPATCHER_H
