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

}

#endif  // SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP