#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::ProcedureExecutor {

// Trivial Executors
template <QP::Types::ClauseType T>
QueryResult executeTrivialNameName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                   const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
template <QP::Types::ClauseType T>
QueryResult executeTrivialWildcardOrSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(const QP::StorageAdapter& storage);
template <QP::Types::ClauseType T>
QueryResult executeTrivialSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                         const Types::ReferenceArgument& rhs);

// Executors
template <QP::Types::ClauseType T>
QueryResult executeNameSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QueryResult executeWildcardSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QueryResult executeSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs);
template <QP::Types::ClauseType T>
QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
template <QP::Types::ClauseType T>
QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                  const Types::ReferenceArgument& rhs);
};

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
