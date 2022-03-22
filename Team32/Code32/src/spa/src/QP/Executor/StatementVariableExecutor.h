#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/Types.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::StatementVariableExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                           const Types::ReferenceArgument& variable);
	static QueryResult executeTrivialIndexWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index);
	static QueryResult executeTrivialSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                             const Types::ReferenceArgument& variable);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index);

	// Executors
	static QueryResult executeSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                      const Types::ReferenceArgument& variable);
	static QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index);
	static QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                         const Types::ReferenceArgument& variable);
	static QueryResult executeIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& index,
	                                       const Types::ReferenceArgument& variable);
};

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_H
