#ifndef SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::PatternContainerStatementExecutor {

using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialName(const StorageAdapter& storage, const ReferenceArgument& var);
template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonym(const StorageAdapter& storage);

// Executors
template <ClauseType T>
QueryResult executeName(const StorageAdapter& storage, const ReferenceArgument& stmt, const ReferenceArgument& var);
template <ClauseType T>
QueryResult executeWildcard(const StorageAdapter& storage, const ReferenceArgument& stmt);
template <ClauseType T>
QueryResult executeSynonym(const StorageAdapter& storage, const ReferenceArgument& stmt, const ReferenceArgument& var);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryName(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcard(const vector<ReferenceArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonym(const vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
