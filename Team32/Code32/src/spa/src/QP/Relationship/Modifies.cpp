#include "QP/Relationship/Modifies.h"

#include <utility>

#include "QP/DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Relationship::Modifies::dispatcher = [](vector<QP::Types::ReferenceArgument> args) {
	return QP::DispatchProcessors::processDoubleArgument(QP::Relationship::Modifies::argument_dispatch_map, move(args));
};