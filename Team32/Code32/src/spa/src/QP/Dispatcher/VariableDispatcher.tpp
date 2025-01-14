#ifndef SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_TPP

#include "QP/Dispatcher/VariableDispatcher.h"

#include <unordered_map>

#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"

namespace QP::Dispatcher::VariableDispatcher {

using namespace QP::Executor;

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getNameMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> map = {
		{ArgumentType::Name, {T, ProcedureVariableExecutor::executorFactoryNameName<T>}},
		{ArgumentType::Wildcard, {T, ProcedureVariableExecutor::executorFactoryNameWildcard<T>}},
		{DesignEntity::Variable, {T, ProcedureVariableExecutor::executorFactoryNameSynonym<T>}},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getIndexMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> map = {
		{ArgumentType::Name, {T, StatementVariableExecutor::executorFactoryIndexName<T>}},
		{ArgumentType::Wildcard, {T, StatementVariableExecutor::executorFactoryIndexWildcard<T>}},
		{DesignEntity::Variable, {T, StatementVariableExecutor::executorFactoryIndexSynonym<T>}},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getProcedureMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> map = {
		{ArgumentType::Name, {T, ProcedureVariableExecutor::executorFactorySynonymName<T>}},
		{ArgumentType::Wildcard, {T, ProcedureVariableExecutor::executorFactorySynonymWildcard<T>}},
		{DesignEntity::Variable, {T, ProcedureVariableExecutor::executorFactorySynonymSynonym<T>}},
	};
	return map;
}

template <ClauseType T>
const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>& getStatementMap() {
	static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> map = {
		{ArgumentType::Name, {T, StatementVariableExecutor::executorFactorySynonymName<T>}},
		{ArgumentType::Wildcard, {T, StatementVariableExecutor::executorFactorySynonymWildcard<T>}},
		{DesignEntity::Variable, {T, StatementVariableExecutor::executorFactorySynonymSynonym<T>}},
	};
	return map;
}

}

#endif  // SPA_SRC_QP_DISPATCHER_VARIABLEDISPATCHER_TPP