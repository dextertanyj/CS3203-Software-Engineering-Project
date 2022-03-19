#include "QP/Dispatcher/PatternAssignDispatcher.h"

#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"

const QP::Types::ArgumentDispatcher QP::Dispatcher::PatternAssignDispatcher::dispatcher =
	[](const vector<QP::Types::ReferenceArgument>& args) {
		return QP::Dispatcher::DispatchProcessors::processArgument(QP::Types::ClauseType::PatternAssign,
	                                                               QP::Dispatcher::PatternAssignDispatcher::argument_dispatch_map, args);
	};