#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H

#include "QP/ClauseArgument.h"
#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::StatementVariableExecutor {

using namespace std;
using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialIndexName(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable);
template <ClauseType T>
QueryResult executeTrivialIndexWildcardOrSynonym(const StorageAdapter& store, const ClauseArgument& index);
template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable);
template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& store, const ClauseArgument& index);

// Executors
template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& index);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable);
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryIndexName(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ClauseArgument>& args);
}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
