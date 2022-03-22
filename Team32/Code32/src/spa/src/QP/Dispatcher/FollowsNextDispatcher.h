#ifndef SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H

#include <unordered_map>
#include <vector>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

template <QP::Types::ClauseType T>
class QP::Dispatcher::FollowsNextDispatcher {
public:
	static Types::ArgumentDispatcher dispatcher;

private:
	static Types::ExecutorSetBundle argumentDispatcher(Types::ClauseType type, const vector<Types::ReferenceArgument>& args);
	static const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>
		argument_dispatch_map;
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getIndexMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getWildcardMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getSynonymMap();
};

#endif  // SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_H
