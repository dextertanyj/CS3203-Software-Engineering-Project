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
	static QueryResult executeTrivialNameName(QP::StorageAdapter& storage, const Types::ReferenceArgument& left_ent,
	                                          const Types::ReferenceArgument& right_ent);
	static QueryResult executeTrivialNameWildcardOrSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& left_ent);
	static QueryResult executeTrivialSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& right_ent);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter& storage);

	// Executors
	static QueryResult executeNameSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& left_ent,
	                                      const Types::ReferenceArgument& right_ent);
	static QueryResult executeSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& left_ent,
	                                      const Types::ReferenceArgument& right_ent);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& left_ent);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& left_ent,
	                                         const Types::ReferenceArgument& right_ent);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H
