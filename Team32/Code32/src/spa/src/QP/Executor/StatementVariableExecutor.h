#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::StatementVariableExecutor {

using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialIndexName(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeTrivialIndexWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& index);
template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& index);

// Executors
template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& index);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable);
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryIndexName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args);
}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
