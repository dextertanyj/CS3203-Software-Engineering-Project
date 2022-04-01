#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP

#include "QP/Dispatcher/CallsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureExecutor.tpp"
#include "QP/StorageAdapter.h"

namespace QP::Dispatcher::CallsDispatcher {

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
		{ReferenceType::Name, getNameMap<T>()},
		{ReferenceType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Procedure, getSynonymMap<T>()}};
	return map;
};

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getNameMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::Name, ProcedureExecutor::executorFactoryNameName<T>},
		{ReferenceType::Wildcard, ProcedureExecutor::executorFactoryNameWildcard<T>},
		{DesignEntity::Procedure, ProcedureExecutor::executorFactoryNameSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::Name, ProcedureExecutor::executorFactoryWildcardName<T>},
		{ReferenceType::Wildcard, ProcedureExecutor::executorFactoryWildcardWildcard<T>},
		{DesignEntity::Procedure, ProcedureExecutor::executorFactoryWildcardSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::Name, ProcedureExecutor::executorFactorySynonymName<T>},
		{ReferenceType::Wildcard, ProcedureExecutor::executorFactorySynonymWildcard<T>},
		{DesignEntity::Procedure, ProcedureExecutor::executorFactorySynonymSynonym<T>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
