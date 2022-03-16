#include "QP/Dispatcher/PatternIfDispatcher.h"

#include <utility>

#include "QP/Dispatcher/DispatchProcessors.h"

const QP::Types::ArgumentDispatcher QP::Dispatcher::PatternIfDispatcher::dispatcher = [](const vector<QP::Types::ReferenceArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processPatternIfArgument(QP::Types::ClauseType::PatternIf,
	                                                                    QP::Dispatcher::PatternIfDispatcher::argument_dispatch_map, args);
};
