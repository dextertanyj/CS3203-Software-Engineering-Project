#include "Modifies.h"

#include "QP/DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Relationship::Modifies::dispatcher = [](vector<ReferenceArgument> args) {
	return QP::DispatchProcessors::processDoubleArgument(QP::Relationship::Modifies::argument_dispatch_map, move(args));
};