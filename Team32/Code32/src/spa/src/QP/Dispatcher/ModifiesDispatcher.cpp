#include "ModifiesDispatcher.h"

#include <utility>

#include "DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Dispatcher::ModifiesDispatcher::dispatcher = [](vector<QP::Types::ReferenceArgument> args) {
	return QP::Dispatcher::DispatchProcessors::processDoubleArgument(QP::Dispatcher::ModifiesDispatcher::argument_dispatch_map, move(args));
};