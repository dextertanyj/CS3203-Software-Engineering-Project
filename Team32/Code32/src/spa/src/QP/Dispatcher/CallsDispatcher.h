#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

template <QP::Types::ClauseType T>
class QP::Dispatcher::CallsDispatcher {
public:
	static Types::ArgumentDispatcher dispatcher;

private:
	static Types::ExecutorSetBundle argumentDispatcher(Types::ClauseType type, const vector<Types::ReferenceArgument>& args);
	static const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>
		argument_dispatch_map;
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getNameMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getWildcardMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getSynonymMap();
};

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_H
