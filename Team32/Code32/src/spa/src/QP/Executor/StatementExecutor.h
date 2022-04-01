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
QueryResult executeTrivialIndexIndex(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialIndexWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeTrivialIndexSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialWildcardIndex(const StorageAdapter& storage, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialWildcardWildcard(const StorageAdapter& storage);
template <ClauseType T>
QueryResult executeTrivialWildcardSynonym(const StorageAdapter& storage, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialSynonymIndex(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Executors
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& storage, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymIndex(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

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

};

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
