#ifndef SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP

#include "QP/Dispatcher/StatementDispatcher.h"

#include "QP/Executor/StatementExecutor.tpp"

namespace QP::Dispatcher::StatementDispatcher {

using namespace QP::Executor;

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getIndexMap() {
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
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
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
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
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

template <>
inline const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap<ClauseType::NextT>() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactoryWildcardIndex<ClauseType::NextT>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactoryWildcardWildcard<ClauseType::NextT>},
		{DesignEntity::Stmt, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Call, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Assign, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Print, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Read, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::While, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::If, StatementExecutor::executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
	};
	return map;
}

template <>
inline const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap<ClauseType::NextT>() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, StatementExecutor::executorFactorySynonymIndex<ClauseType::NextT>},
		{ReferenceType::Wildcard, StatementExecutor::executorFactorySynonymWildcardOptimized<ClauseType::NextT>},
		{DesignEntity::Stmt, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Call, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Assign, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Print, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Read, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::While, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::If, StatementExecutor::executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP