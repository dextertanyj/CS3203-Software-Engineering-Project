#include "UsesDispatcher.h"

#include <utility>

#include "DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Dispatcher::UsesDispatcher::dispatcher = [](vector<QP::Types::ReferenceArgument> args) {
	return QP::Dispatcher::DispatchProcessors::processDoubleArgument(QP::Dispatcher::UsesDispatcher::argument_dispatch_map, move(args));
};