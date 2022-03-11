#include "QP/Relationship/Uses.h"

#include <utility>

#include "QP/DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Relationship::Uses::dispatcher = [](vector<Types::ReferenceArgument> args) {
	return QP::DispatchProcessors::processDoubleArgument(QP::Relationship::Uses::argument_dispatch_map, move(args));
};