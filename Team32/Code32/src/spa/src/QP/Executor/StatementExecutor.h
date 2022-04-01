#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::StatementExecutor {

using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialIndexIndex(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialIndexWildcard(const StorageAdapter& store, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeTrivialIndexSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialWildcardIndex(const StorageAdapter& store, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialWildcardWildcard(const StorageAdapter& store);
template <ClauseType T>
QueryResult executeTrivialWildcardSynonym(const StorageAdapter& store, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialSynonymIndex(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Executors
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& store, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymIndex(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Optimized Executors
template <ClauseType T>
QueryResult executeWildcardSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymWildcardOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeSynonymSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ReferenceArgument& lhs,
                                           const ReferenceArgument& rhs);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryIndexIndex(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardIndex(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymIndex(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonymOptimized(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcardOptimized(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonymOptimized(const vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
