#ifndef SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_TPP

#include "QP/Dispatcher/FollowsNextDispatcher.h"

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/StatementDispatcher.h"
#include "QP/Executor/StatementExecutor.tpp"

using namespace QP::Types;
using namespace QP::Executor;

namespace QP::Dispatcher::FollowsNextDispatcher {

template <ClauseType T>
ExecutorSet executorFactoryIndexIndex(const vector<ReferenceArgument>& args) {
	return [lhs = args.at(0), rhs = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementExecutor::executeTrivialIndexIndex<T>(storage, lhs, rhs);
	};
}

template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args) {
	return
		[lhs = args.at(0)](const QP::StorageAdapter& storage) { return StatementExecutor::executeTrivialIndexWildcard<T>(storage, lhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardIndex(const vector<ReferenceArgument>& args) {
	return
		[rhs = args.at(1)](const QP::StorageAdapter& storage) { return StatementExecutor::executeTrivialWildcardIndex<T>(storage, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& /*args*/) {
	return [](const QP::StorageAdapter& storage) { return StatementExecutor::executeTrivialWildcardWildcard<T>(storage); };
}

template <ClauseType T>
ExecutorSet executorFactorySynonymIndex(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementExecutor::executeTrivialSynonymIndex<T>(storage, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementExecutor::executeSynonymIndex<T>(storage, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementExecutor::executeTrivialSynonymWildcard<T>(storage, lhs);
	};
	Types::Executor executor = [lhs = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementExecutor::executeSynonymWildcard<T>(storage, lhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getIndexMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, executorFactoryIndexIndex<T>},
		{ReferenceType::Wildcard, executorFactoryIndexWildcard<T>},
		{DesignEntity::Stmt, lambdaIndexSynonym<T>()},
		{DesignEntity::Call, lambdaIndexSynonym<T>()},
		{DesignEntity::Assign, lambdaIndexSynonym<T>()},
		{DesignEntity::Print, lambdaIndexSynonym<T>()},
		{DesignEntity::Read, lambdaIndexSynonym<T>()},
		{DesignEntity::While, lambdaIndexSynonym<T>()},
		{DesignEntity::If, lambdaIndexSynonym<T>()},
	};
	return map;
}

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, executorFactoryWildcardIndex<T>},
		{ReferenceType::Wildcard, executorFactoryWildcardWildcard<T>},
		{DesignEntity::Stmt, lambdaWildcardSynonym<T>()},
		{DesignEntity::Call, lambdaWildcardSynonym<T>()},
		{DesignEntity::Assign, lambdaWildcardSynonym<T>()},
		{DesignEntity::Print, lambdaWildcardSynonym<T>()},
		{DesignEntity::Read, lambdaWildcardSynonym<T>()},
		{DesignEntity::While, lambdaWildcardSynonym<T>()},
		{DesignEntity::If, lambdaWildcardSynonym<T>()},
	};
	return map;
}

template <ClauseType T>
unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::StatementIndex, executorFactorySynonymIndex<T>},
		{ReferenceType::Wildcard, executorFactorySynonymWildcard<T>},
		{DesignEntity::Stmt, lambdaSynonymSynonym<T>()},
		{DesignEntity::Call, lambdaSynonymSynonym<T>()},
		{DesignEntity::Assign, lambdaSynonymSynonym<T>()},
		{DesignEntity::Print, lambdaSynonymSynonym<T>()},
		{DesignEntity::Read, lambdaSynonymSynonym<T>()},
		{DesignEntity::While, lambdaSynonymSynonym<T>()},
		{DesignEntity::If, lambdaSynonymSynonym<T>()},
	};
	return map;
}

template <ClauseType T>
static unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> getArgumentDispatchMap() {
	static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> map = {
		{ReferenceType::StatementIndex, getIndexMap<T>()},
		{ReferenceType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Stmt, getSynonymMap<T>()},
		{DesignEntity::Call, getSynonymMap<T>()},
		{DesignEntity::Assign, getSynonymMap<T>()},
		{DesignEntity::Print, getSynonymMap<T>()},
		{DesignEntity::Read, getSynonymMap<T>()},
		{DesignEntity::While, getSynonymMap<T>()},
		{DesignEntity::If, getSynonymMap<T>()}};
	return map;
}

template <ClauseType T>
static ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ReferenceArgument>& args) {
	static const auto map = getArgumentDispatchMap<T>();
	return DispatchProcessors::processArgument(type, map, args);
}

}

template <ClauseType T>
ExecutorSetBundle QP::Dispatcher::FollowsNextDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return argumentDispatcher<T>(T, args);
};

#endif  // SPA_SRC_QP_DISPATCHER_FOLLOWSNEXTDISPATCHER_TPP