#ifndef SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_TPP

#include "QP/Dispatcher/FollowsNextDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/StatementDispatcher.tpp"

namespace QP::Dispatcher::FollowsNextDispatcher {

using namespace std;
using namespace QP::Dispatcher::StatementDispatcher;

template <ClauseType T>
ExecutorSetBundle dispatcher(const vector<ClauseArgument>& args) {
	return argumentDispatcher<T>(T, args);
}

template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ClauseArgument>& args) {
	static const auto map = getArgumentDispatchMap<T>();
	return DispatchProcessors::processArgument(type, map, args);
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>& getArgumentDispatchMap() {
	static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> map = {
		{ArgumentType::Number, getIndexMap<T>()}, {ArgumentType::Wildcard, getWildcardMap<T>()}, {DesignEntity::Stmt, getSynonymMap<T>()},
		{DesignEntity::Call, getSynonymMap<T>()}, {DesignEntity::Assign, getSynonymMap<T>()},    {DesignEntity::Print, getSynonymMap<T>()},
		{DesignEntity::Read, getSynonymMap<T>()}, {DesignEntity::While, getSynonymMap<T>()},     {DesignEntity::If, getSynonymMap<T>()}};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_TPP