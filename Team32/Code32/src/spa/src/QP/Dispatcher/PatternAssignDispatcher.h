#ifndef SPA_SRC_QP_DISPATCHER_PATTERNASSIGNDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PATTERNASSIGNDISPATCHER_H

#include <vector>

#include "QP/ClauseArgument.h"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Types.h"

namespace QP::Dispatcher::PatternAssignDispatcher {

using namespace QP::Types;

ExecutorSetBundle dispatcher(std::vector<ClauseArgument>& args);
};

#endif  // SPA_SRC_QP_DISPATCHER_PATTERNASSIGNDISPATCHER_H
