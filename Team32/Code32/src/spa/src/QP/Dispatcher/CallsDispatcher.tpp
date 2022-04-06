#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP

#include "QP/Dispatcher/CallsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureExecutor.tpp"
#include "QP/StorageAdapter.h"

namespace QP::Dispatcher::CallsDispatcher {

using namespace std;
using namespace QP::Executor;

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
		{ArgumentType::Name, getNameMap<T>()},
		{ArgumentType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Procedure, getSynonymMap<T>()}};
	return map;
};

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getNameMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Name, ProcedureExecutor::executorFactoryNameName<T>},
		{ArgumentType::Wildcard, ProcedureExecutor::executorFactoryNameWildcard<T>},
		{DesignEntity::Procedure, ProcedureExecutor::executorFactoryNameSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Name, ProcedureExecutor::executorFactoryWildcardName<T>},
		{ArgumentType::Wildcard, ProcedureExecutor::executorFactoryWildcardWildcard<T>},
		{DesignEntity::Procedure, ProcedureExecutor::executorFactoryWildcardSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Name, ProcedureExecutor::executorFactorySynonymName<T>},
		{ArgumentType::Wildcard, ProcedureExecutor::executorFactorySynonymWildcard<T>},
		{DesignEntity::Procedure, ProcedureExecutor::executorFactorySynonymSynonym<T>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
