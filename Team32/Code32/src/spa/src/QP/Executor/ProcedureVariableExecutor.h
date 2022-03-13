#ifndef SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H
#define SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::ProcedureVariableExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                          const Types::ReferenceArgument& variable);
	static QueryResult executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure);
	static QueryResult executeTrivialSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& variable);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter& storage);

	// Executors
	static QueryResult executeNameSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                      const Types::ReferenceArgument& variable);
	static QueryResult executeSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                      const Types::ReferenceArgument& variable);
	static QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure);
	static QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                         const Types::ReferenceArgument& variable);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H
