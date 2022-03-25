#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::StatementVariableExecutor {

// Trivial Executors

template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialIndexName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                           const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeTrivialIndexWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index);
template <QP::Types::ClauseType T>
QueryResult executeTrivialSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                             const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index);

// Executors

template <QP::Types::ClauseType T>
QueryResult executeSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                      const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index);
template <QP::Types::ClauseType T>
QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                         const Types::ReferenceArgument& variable);
template <QP::Types::ClauseType T>
QueryResult executeIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                       const Types::ReferenceArgument& variable);
}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
