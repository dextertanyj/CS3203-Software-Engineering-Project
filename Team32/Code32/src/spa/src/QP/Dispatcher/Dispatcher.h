#ifndef SPA_DISPATCHER_H
#define SPA_DISPATCHER_H

#include "QP/QueryTypes.h"

namespace QP::Dispatcher {
struct DispatchMap;

namespace DispatchProcessors {};

template <QP::Types::ClauseType T>
class FollowsDispatcher;

template <QP::Types::ClauseType T>
class ParentDispatcher;

template <QP::Types::ClauseType T>
class CallsDispatcher;

namespace Modifies {};
namespace Uses {};
}  // namespace QP::Dispatcher

#endif  // SPA_DISPATCHER_H
