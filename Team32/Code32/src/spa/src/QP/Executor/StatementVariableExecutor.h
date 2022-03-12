#ifndef SPA_STATEMENTVARIABLEEXECUTOR_H
#define SPA_STATEMENTVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::StatementVariableExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexName(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                                           const Types::ReferenceArgument& ent);
	static QueryResult executeTrivialIndexWildcardOrSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt);
	static QueryResult executeTrivialSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                                             const Types::ReferenceArgument& ent);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt);

	// Executors
	static QueryResult executeSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                                      const Types::ReferenceArgument& ent);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                                         const Types::ReferenceArgument& ent);
	static QueryResult executeIndexSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                                       const Types::ReferenceArgument& ent);
};

#endif  // SPA_STATEMENTVARIABLEEXECUTOR_H
