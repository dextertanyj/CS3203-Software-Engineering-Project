#ifndef SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::PatternContainerStatementExecutor {

// Trivial Executors
template <QP::Types::ClauseType T>
QueryResult executeTrivialName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& var);
template <QP::Types::ClauseType T>
QueryResult executeTrivialWildcardOrSynonym(const QP::StorageAdapter& storage);

// Executors
template <QP::Types::ClauseType T>
QueryResult executeName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt, const Types::ReferenceArgument& var);
template <QP::Types::ClauseType T>
QueryResult executeWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt);
template <QP::Types::ClauseType T>
QueryResult executeSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt, const Types::ReferenceArgument& var);
};

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
