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
QueryResult executeTrivialNameName(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& store, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymName(const StorageAdapter& store, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(const StorageAdapter& store);
template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Executors
template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& store, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);
template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& lhs);
template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryNameName(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardName(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymName(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const std::vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const std::vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
