#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHER_H

#include "QP/Types.h"

namespace QP::Dispatcher {
struct DispatchMap;

namespace DispatchProcessors {};

namespace FollowsNextDispatcher {};

template <QP::Types::ClauseType T>
class ParentDispatcher;

namespace CallsDispatcher {}

namespace WithDispatcher {}
}

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHER_H
