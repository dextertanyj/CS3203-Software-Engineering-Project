#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::StatementExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                            const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialIndexWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                              const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardWildcard(const QP::StorageAdapter& storage);
	static QueryResult executeTrivialWildcardSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialSynonymIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                              const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                                const Types::ReferenceArgument& rhs);

	// Executors
	static QueryResult executeIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                       const Types::ReferenceArgument& rhs);
	static QueryResult executeWildcardSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                       const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                         const Types::ReferenceArgument& rhs);
};

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTEXECUTOR_H
