#ifndef SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP

#include "QP/Dispatcher/CallsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureExecutor.tpp"
#include "QP/StorageAdapter.h"

using namespace QP::Types;
using namespace QP::Executor;

namespace QP::Dispatcher::CallsDispatcher {

template <ClauseType T>
static ExecutorSet executorFactoryNameName(const vector<ReferenceArgument>& args) {
	return [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialNameName<T>(pkb, caller, callee);
	};
}

template <ClauseType T>
static ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args) {
	return [caller = args.at(0)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialNameWildcardOrSynonym<T>(pkb, caller);
	};
}

template <ClauseType T>
static ExecutorSet executorFactoryNameSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [caller = args.at(0)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialNameWildcardOrSynonym<T>(pkb, caller);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeNameSynonym<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
static ExecutorSet executorFactoryWildcardName(const vector<ReferenceArgument>& args) {
	return [callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialWildcardOrSynonymName<T>(pkb, callee);
	};
}

template <ClauseType T>
static ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& /*args*/) {
	return [](const QP::StorageAdapter& pkb) { return ProcedureExecutor::executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
}

template <ClauseType T>
static ExecutorSet executorFactoryWildcardSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeWildcardSynonym<T>(pkb, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
static ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialWildcardOrSynonymName<T>(pkb, callee);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeSynonymName<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
static ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb);
	};
	Types::Executor executor = [caller = args.at(0)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeSynonymWildcard<T>(pkb, caller);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
static ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeTrivialSynonymSynonym<T>(pkb, caller, callee);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
		return ProcedureExecutor::executeSynonymSynonym<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
static unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getNameMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::Name, executorFactoryNameName<T>},
		{ReferenceType::Wildcard, executorFactoryNameWildcard<T>},
		{DesignEntity::Procedure, executorFactoryNameSynonym<T>},
	};
	return map;
}

template <ClauseType T>
static unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getWildcardMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::Name, executorFactoryWildcardName<T>},
		{ReferenceType::Wildcard, executorFactoryWildcardWildcard<T>},
		{DesignEntity::Procedure, executorFactoryWildcardSynonym<T>},
	};
	return map;
}

template <ClauseType T>
static unordered_map<ArgumentDispatchKey, ExecutorSetFactory> getSynonymMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map = {
		{ReferenceType::Name, executorFactorySynonymName<T>},
		{ReferenceType::Wildcard, executorFactorySynonymWildcard<T>},
		{DesignEntity::Procedure, executorFactorySynonymSynonym<T>},
	};
	return map;
}

template <ClauseType T>
static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> getArgumentDispatchMap() {
	static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> map = {
		{ReferenceType::Name, getNameMap<T>()},
		{ReferenceType::Wildcard, getWildcardMap<T>()},
		{DesignEntity::Procedure, getSynonymMap<T>()}};
	return map;
};

template <ClauseType T>
static ExecutorSetBundle argumentDispatcher(ClauseType type, const vector<ReferenceArgument>& args) {
	static const auto map = getArgumentDispatchMap<T>();
	return QP::Dispatcher::DispatchProcessors::processArgument(type, map, args);
}

}

template <ClauseType T>
ExecutorSetBundle QP::Dispatcher::CallsDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return argumentDispatcher<T>(T, args);
}

#endif  // SPA_SRC_QP_DISPATCHER_CALLSDISPATCHER_TPP