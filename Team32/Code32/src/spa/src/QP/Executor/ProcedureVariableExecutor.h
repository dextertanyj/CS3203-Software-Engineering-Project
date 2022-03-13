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
	static QueryResult executeTrivialNameName(QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                          const Types::ReferenceArgument& variable);
	static QueryResult executeTrivialNameWildcardOrSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure);
	static QueryResult executeTrivialSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& variable);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter& storage);

	// Executors
	static QueryResult executeNameSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                      const Types::ReferenceArgument& variable);
	static QueryResult executeSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                      const Types::ReferenceArgument& variable);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& procedure,
	                                         const Types::ReferenceArgument& variable);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H
