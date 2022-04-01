#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::ProcedureVariableExecutor {

using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialNameName(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& procedure);
template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& storage, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& storage);

// Executors
template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& procedure);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryNameName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args);
}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_H
