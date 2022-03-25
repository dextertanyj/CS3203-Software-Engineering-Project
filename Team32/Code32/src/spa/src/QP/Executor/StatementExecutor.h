#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::StatementExecutor {

// Trivial Executors

template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialIndexIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                     const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialIndexWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                           const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialWildcardIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialWildcardWildcard(const QP::StorageAdapter& storage);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialWildcardSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialSynonymIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                       const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeTrivialSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                         const Types::ReferenceArgument& rhs);

// Executors

template <QP::Types::ClauseType T>
QP::QueryResult executeIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeWildcardSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeSynonymIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
template <QP::Types::ClauseType T>
QP::QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                  const Types::ReferenceArgument& rhs);
};

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
