#ifndef SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H

#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::PatternIfDispatcher {

using namespace QP::Types;

ExecutorSetBundle dispatcher(const vector<ReferenceArgument>& args);
}

#endif  // SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H
