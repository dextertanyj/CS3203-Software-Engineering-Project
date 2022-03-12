#ifndef SPA_STATEMENTRELATIONEXECUTOR_H
#define SPA_STATEMENTRELATIONEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::StatementRelationExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexIndex(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                            const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialIndexWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialIndexSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                              const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardIndex(QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardWildcard(QP::StorageAdapter& storage);
	static QueryResult executeTrivialWildcardSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialSynonymIndex(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                              const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                                const Types::ReferenceArgument& rhs);

	// Executors
	static QueryResult executeIndexSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                       const Types::ReferenceArgument& rhs);
	static QueryResult executeWildcardSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymIndex(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                       const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                         const Types::ReferenceArgument& rhs);
};

#endif  // SPA_STATEMENTRELATIONEXECUTOR_H
