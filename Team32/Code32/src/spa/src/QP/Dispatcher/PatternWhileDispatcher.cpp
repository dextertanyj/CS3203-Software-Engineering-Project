#include "QP/Dispatcher/PatternWhileDispatcher.h"

#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"

const QP::Types::ArgumentDispatcher QP::Dispatcher::PatternWhileDispatcher::dispatcher =
	[](const vector<QP::Types::ReferenceArgument>& args) {
		return QP::Dispatcher::DispatchProcessors::processArgument(QP::Types::ClauseType::PatternWhile,
	                                                               QP::Dispatcher::PatternWhileDispatcher::argument_dispatch_map, args);
	};
