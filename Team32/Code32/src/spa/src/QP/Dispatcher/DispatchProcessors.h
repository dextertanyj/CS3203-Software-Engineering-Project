#ifndef SPA_SRC_QP_DISPATCHPROCESSORS_H
#define SPA_SRC_QP_DISPATCHPROCESSORS_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::DispatchProcessors {
Types::ExecutorSetBundle processSingleArgument(Types::ClauseType type,
                                               unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> map,
                                               const vector<Types::ReferenceArgument>& args);
Types::ExecutorSetBundle processDoubleArgument(
	Types::ClauseType type,
	unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>> map,
	const vector<Types::ReferenceArgument>& args);
Types::ExecutorSetBundle processDoubleArgument(
	unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle>> map,
	const vector<Types::ReferenceArgument>& args);
Types::ExecutorSetBundle processTripleArgument(
	Types::ClauseType type,
	unordered_map<Types::ArgumentDispatchKey,
                  unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>>
		map,
	const vector<Types::ReferenceArgument>& args);
};  // namespace QP::Dispatcher::DispatchProcessors

#endif  // SPA_SRC_QP_DISPATCHPROCESSORS_H
