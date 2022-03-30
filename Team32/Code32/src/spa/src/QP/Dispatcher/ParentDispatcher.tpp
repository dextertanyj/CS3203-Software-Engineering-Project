#ifndef SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP

#include "QP/Dispatcher/ParentDispatcher.h"

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/StatementExecutor.tpp"

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
unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> getArgumentDispatchMap() {
	static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> map = {
		{ReferenceType::StatementIndex, getIndexMap<T>()},
		{ReferenceType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Stmt, getSynonymMap<T>()},
		{DesignEntity::While, getSynonymMap<T>()},
		{DesignEntity::If, getSynonymMap<T>()}};
	return map;
}

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getIndexMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactoryIndexIndex<T>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactoryIndexWildcard<T>},
		{DesignEntity::Stmt, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::Call, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::Assign, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::Print, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::Read, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::While, StatementExecutor::executorFactoryIndexSynonym<T>},
		{DesignEntity::If, StatementExecutor::executorFactoryIndexSynonym<T>},
	};
	return map;
}

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactoryWildcardIndex<T>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactoryWildcardWildcard<T>},
		{DesignEntity::Stmt, StatementExecutor::executorFactoryWildcardSynonym<T>},
		{DesignEntity::Call, StatementExecutor::executorFactoryWildcardSynonym<T>},
		{DesignEntity::Assign, StatementExecutor::executorFactoryWildcardSynonym<T>},
		{DesignEntity::Print, StatementExecutor::executorFactoryWildcardSynonym<T>},
		{DesignEntity::Read, StatementExecutor::executorFactoryWildcardSynonym<T>},
		{DesignEntity::While, StatementExecutor::executorFactoryWildcardSynonym<T>},
		{DesignEntity::If, StatementExecutor::executorFactoryWildcardSynonym<T>},
	};
	return map;
}

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactorySynonymIndex<T>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactorySynonymWildcard<T>},
		{DesignEntity::Stmt, StatementExecutor::executorFactorySynonymSynonym<T>},
		{DesignEntity::Call, StatementExecutor::executorFactorySynonymSynonym<T>},
		{DesignEntity::Assign, StatementExecutor::executorFactorySynonymSynonym<T>},
		{DesignEntity::Print, StatementExecutor::executorFactorySynonymSynonym<T>},
		{DesignEntity::Read, StatementExecutor::executorFactorySynonymSynonym<T>},
		{DesignEntity::While, StatementExecutor::executorFactorySynonymSynonym<T>},
		{DesignEntity::If, StatementExecutor::executorFactorySynonymSynonym<T>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP