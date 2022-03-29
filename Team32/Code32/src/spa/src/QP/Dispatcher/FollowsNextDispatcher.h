#ifndef SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::FollowsNextDispatcher {
template <QP::Types::ClauseType T>
Types::ExecutorSetBundle dispatcher(const vector<Types::ReferenceArgument>& arguments);
}

#endif  // SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
