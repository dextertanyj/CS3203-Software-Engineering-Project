#ifndef SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_TPP

#include "QP/Dispatcher/AffectsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/StatementExecutor.tpp"

namespace QP::Dispatcher::AffectsDispatcher {

using namespace QP::Executor::StatementExecutor;

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
		{ArgumentType::Number, getIndexMap<T>()},
		{ArgumentType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Stmt, getSynonymMap<T>()},
		{DesignEntity::Assign, getSynonymMap<T>()}};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getIndexMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactoryIndexIndex<T>},
		{ArgumentType::Wildcard, executorFactoryIndexWildcard<T>},
		{DesignEntity::Stmt, executorFactoryIndexSynonym<T>},
		{DesignEntity::Assign, executorFactoryIndexSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactoryWildcardIndex<T>},
		{ArgumentType::Wildcard, executorFactoryWildcardWildcard<T>},
		{DesignEntity::Stmt, executorFactoryWildcardSynonymOptimized<T>},
		{DesignEntity::Assign, executorFactoryWildcardSynonymOptimized<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactorySynonymIndex<T>},
		{ArgumentType::Wildcard, executorFactorySynonymWildcardOptimized<T>},
		{DesignEntity::Stmt, executorFactorySynonymSynonymOptimized<T>},
		{DesignEntity::Assign, executorFactorySynonymSynonymOptimized<T>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_TPP