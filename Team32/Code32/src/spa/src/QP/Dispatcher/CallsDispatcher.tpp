#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP

#include "QP/Dispatcher/CallsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureExecutor.tpp"
#include "QP/StorageAdapter.h"

namespace QP::Dispatcher::CallsDispatcher {

using namespace std;
using namespace QP::Executor::ProcedureExecutor;

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
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getNameMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Name, executorFactoryNameName<T>},
		{ArgumentType::Wildcard, executorFactoryNameWildcard<T>},
		{DesignEntity::Procedure, executorFactoryNameSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Name, executorFactoryWildcardName<T>},
		{ArgumentType::Wildcard, executorFactoryWildcardWildcard<T>},
		{DesignEntity::Procedure, executorFactoryWildcardSynonym<T>},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactory>& getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ArgumentType::Name, executorFactorySynonymName<T>},
		{ArgumentType::Wildcard, executorFactorySynonymWildcard<T>},
		{DesignEntity::Procedure, executorFactorySynonymSynonym<T>},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
