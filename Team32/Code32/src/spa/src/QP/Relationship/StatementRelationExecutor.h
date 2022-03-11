#ifndef SPA_STATEMENTRELATIONEXECUTOR_H
#define SPA_STATEMENTRELATIONEXECUTOR_H

#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/StorageAdapter.h"
#include "QP/ReferenceArgument.h"

namespace QP::Relationship {
template <QP::Types::ClauseType T>
class StatementRelationExecutor;
}

template <QP::Types::ClauseType T>
class QP::Relationship::StatementRelationExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexIndex(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                            const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialIndexWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialIndexSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                              const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardIndex(QP::StorageAdapter& pkb, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardWildcard(QP::StorageAdapter& pkb);
	static QueryResult executeTrivialWildcardSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialSynonymIndex(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                              const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                                const Types::ReferenceArgument& rhs);

	// Executors
	static QueryResult executeIndexSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                       const Types::ReferenceArgument& rhs);
	static QueryResult executeWildcardSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymIndex(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                       const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs,
	                                         const Types::ReferenceArgument& rhs);
};

#endif  // SPA_STATEMENTRELATIONEXECUTOR_H
