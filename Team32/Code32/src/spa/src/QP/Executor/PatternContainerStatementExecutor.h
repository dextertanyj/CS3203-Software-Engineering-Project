#ifndef SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H

#include "QP/ClauseArgument.h"
#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::PatternContainerStatementExecutor {

using namespace std;
using namespace QP::Types;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialName(const StorageAdapter& store, const ClauseArgument& var);
template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonym(const StorageAdapter& store);

// Executors
template <ClauseType T>
QueryResult executeName(const StorageAdapter& store, const ClauseArgument& stmt, const ClauseArgument& var);
template <ClauseType T>
QueryResult executeWildcard(const StorageAdapter& store, const ClauseArgument& stmt);
template <ClauseType T>
QueryResult executeSynonym(const StorageAdapter& store, const ClauseArgument& stmt, const ClauseArgument& var);

// Executor Set Factories
template <ClauseType T>
ExecutorSet executorFactoryName(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactoryWildcard(const vector<ClauseArgument>& args);
template <ClauseType T>
ExecutorSet executorFactorySynonym(const vector<ClauseArgument>& args);
}

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
