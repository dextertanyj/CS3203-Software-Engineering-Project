#ifndef SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H
#define SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H

#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

namespace QP::Relationship {
template <QP::Types::ClauseType T>
class ProcedureVariableExecutor;
}

template <QP::Types::ClauseType T>
class QP::Relationship::ProcedureVariableExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& left_ent,
	                                          const Types::ReferenceArgument& right_ent);
	static QueryResult executeTrivialNameWildcardOrSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& left_ent);
	static QueryResult executeTrivialSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& right_ent);
	static QueryResult executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter& pkb);

	// Executors
	static QueryResult executeNameSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& left_ent,
	                                      const Types::ReferenceArgument& right_ent);
	static QueryResult executeSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& left_ent,
	                                      const Types::ReferenceArgument& right_ent);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& left_ent);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& left_ent,
	                                         const Types::ReferenceArgument& right_ent);
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_H
