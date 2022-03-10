#include "Uses.h"

#include "QP/DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Relationship::Uses::dispatcher = [](vector<ReferenceArgument> args) {
	return QP::DispatchProcessors::processDoubleArgument(QP::Relationship::Uses::argument_dispatch_map, move(args));
};