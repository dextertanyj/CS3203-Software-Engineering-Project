#ifndef SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H

#include <vector>

#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

namespace QP::Dispatcher::ModifiesDispatcher {
Types::ExecutorSetBundle dispatcher(const vector<Types::ReferenceArgument>& arguments);
};

#endif  // SPA_SRC_QP_DISPATCHER_MODIFIESDISPATCHER_H
