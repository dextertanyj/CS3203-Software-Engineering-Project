#ifndef SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP

#include "QP/Dispatcher/ParentDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/StatementDispatcher.tpp"

namespace QP::Dispatcher::ParentDispatcher {

using namespace QP::Executor;

template <ClauseType T>
ExecutorSetBundle dispatcher(const vector<ReferenceArgument>& args) {
	return argumentDispatcher<T>(T, args);
}

template <ClauseType T>
ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ReferenceArgument>& args) {
	static const auto map = getArgumentDispatchMap<T>();
	return DispatchProcessors::processArgument(type, map, args);
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>& getArgumentDispatchMap() {
	static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> map = {
		{ReferenceType::StatementIndex, StatementDispatcher::getIndexMap<T>()},
		{ReferenceType::Wildcard, StatementDispatcher::getWildcardMap<T>()},
		{DesignEntity::Stmt, StatementDispatcher::getSynonymMap<T>()},
		{DesignEntity::While, StatementDispatcher::getSynonymMap<T>()},
		{DesignEntity::If, StatementDispatcher::getSynonymMap<T>()}};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP