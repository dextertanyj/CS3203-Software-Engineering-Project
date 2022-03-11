#ifndef SPA_STATEMENTVARIABLEEXECUTOR_H
#define SPA_STATEMENTVARIABLEEXECUTOR_H

#include "QP/QueryTypes.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"
#include "QP/ReferenceArgument.h"

namespace QP::Relationship {
template <QP::Types::ClauseType T>
class StatementVariableExecutor;
}

template <QP::Types::ClauseType T>
class QP::Relationship::StatementVariableExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialIndexName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt,
	                                           const Types::ReferenceArgument& ent);
	static QueryResult executeTrivialIndexWildcardOrSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt);
	static QueryResult executeTrivialSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt,
	                                             const Types::ReferenceArgument& ent);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt);

	// Executors
	static QueryResult executeSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt,
	                                      const Types::ReferenceArgument& ent);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt,
	                                         const Types::ReferenceArgument& ent);
	static QueryResult executeIndexSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& stmt,
	                                       const Types::ReferenceArgument& ent);
};

#endif  // SPA_STATEMENTVARIABLEEXECUTOR_H
