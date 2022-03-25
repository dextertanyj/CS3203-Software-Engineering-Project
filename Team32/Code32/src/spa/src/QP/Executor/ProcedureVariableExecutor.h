#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::ProcedureVariableExecutor {
// Trivial Executors
template <QP::Types::ClauseType T>
QueryResult executeTrivialNameName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
                                   const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure);
template <QP::Types::ClauseType T>
QueryResult executeTrivialSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter& storage);

// Executors
template <QP::Types::ClauseType T>
QueryResult executeNameSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
                               const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
                               const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure);
template <QP::Types::ClauseType T>
QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
                                  const Types::ReferenceArgument& variable);
};

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_H
