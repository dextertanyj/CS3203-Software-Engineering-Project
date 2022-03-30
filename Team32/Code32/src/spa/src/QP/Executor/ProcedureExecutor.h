#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::ProcedureExecutor {

using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialNameName(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymName(const StorageAdapter& storage, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(const StorageAdapter& storage);
template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Executors
template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& storage, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryNameName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
