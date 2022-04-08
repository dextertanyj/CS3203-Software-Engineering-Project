#ifndef SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP

#include "QP/Dispatcher/StatementDispatcher.h"

#include "QP/Executor/StatementExecutor.tpp"

namespace QP::Dispatcher::StatementDispatcher {

using namespace QP::Executor::StatementExecutor;

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getIndexMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactoryIndexIndex<T>},   {ArgumentType::Wildcard, executorFactoryIndexWildcard<T>},
		{DesignEntity::Stmt, executorFactoryIndexSynonym<T>},   {DesignEntity::Call, executorFactoryIndexSynonym<T>},
		{DesignEntity::Assign, executorFactoryIndexSynonym<T>}, {DesignEntity::Print, executorFactoryIndexSynonym<T>},
		{DesignEntity::Read, executorFactoryIndexSynonym<T>},   {DesignEntity::While, executorFactoryIndexSynonym<T>},
		{DesignEntity::If, executorFactoryIndexSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactoryWildcardIndex<T>},   {ArgumentType::Wildcard, executorFactoryWildcardWildcard<T>},
		{DesignEntity::Stmt, executorFactoryWildcardSynonym<T>},   {DesignEntity::Call, executorFactoryWildcardSynonym<T>},
		{DesignEntity::Assign, executorFactoryWildcardSynonym<T>}, {DesignEntity::Print, executorFactoryWildcardSynonym<T>},
		{DesignEntity::Read, executorFactoryWildcardSynonym<T>},   {DesignEntity::While, executorFactoryWildcardSynonym<T>},
		{DesignEntity::If, executorFactoryWildcardSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactorySynonymIndex<T>},   {ArgumentType::Wildcard, executorFactorySynonymWildcard<T>},
		{DesignEntity::Stmt, executorFactorySynonymSynonym<T>},   {DesignEntity::Call, executorFactorySynonymSynonym<T>},
		{DesignEntity::Assign, executorFactorySynonymSynonym<T>}, {DesignEntity::Print, executorFactorySynonymSynonym<T>},
		{DesignEntity::Read, executorFactorySynonymSynonym<T>},   {DesignEntity::While, executorFactorySynonymSynonym<T>},
		{DesignEntity::If, executorFactorySynonymSynonym<T>},
	};
	return map;
}

template <>
inline const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap<ClauseType::NextT>() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactoryWildcardIndex<ClauseType::NextT>},
		{ArgumentType::Wildcard, executorFactoryWildcardWildcard<ClauseType::NextT>},
		{DesignEntity::Stmt, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Call, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Assign, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Print, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Read, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::While, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::If, executorFactoryWildcardSynonymOptimized<ClauseType::NextT>},
	};
	return map;
}

template <>
inline const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap<ClauseType::NextT>() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Number, executorFactorySynonymIndex<ClauseType::NextT>},
		{ArgumentType::Wildcard, executorFactorySynonymWildcardOptimized<ClauseType::NextT>},
		{DesignEntity::Stmt, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Call, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Assign, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Print, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::Read, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::While, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
		{DesignEntity::If, executorFactorySynonymSynonymOptimized<ClauseType::NextT>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_STATEMENTDISPATCHER_TPP