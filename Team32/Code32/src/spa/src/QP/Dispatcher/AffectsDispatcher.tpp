#ifndef SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_TPP

#include "QP/Dispatcher/AffectsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/StatementExecutor.tpp"
#include "QP/StorageAdapter.h"

namespace QP::Dispatcher::AffectsDispatcher {

using namespace std;
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
		{ReferenceType::StatementIndex, getIndexMap<T>()},
		{ReferenceType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Stmt, getSynonymMap<T>()},
		{DesignEntity::Assign, getSynonymMap<T>()}};
	return map;
};

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getIndexMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactoryIndexIndex<T>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactoryIndexWildcard<T>},
		{DesignEntity::Stmt, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::Assign, StatementExecutor::executorFactoryIndexSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactoryWildcardIndex<T>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactoryWildcardWildcard<T>},
		{DesignEntity::Stmt, StatementExecutor::executorFactoryWildcardSynonymOptimized<T>},
		{DesignEntity::Assign, StatementExecutor::executorFactoryWildcardSynonymOptimized<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactorySynonymIndex<T>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactorySynonymWildcardOptimized<T>},
		{DesignEntity::Stmt, StatementExecutor::executorFactorySynonymSynonymOptimized<T>},
		{DesignEntity::Assign, StatementExecutor::executorFactorySynonymSynonymOptimized<T>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_AFFECTSDISPATCHER_TPP