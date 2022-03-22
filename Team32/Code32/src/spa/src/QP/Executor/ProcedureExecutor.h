#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/Types.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::ProcedureExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                          const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeTrivialWildcardOrSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(const QP::StorageAdapter& storage);
	static QueryResult executeTrivialSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                                const Types::ReferenceArgument& rhs);

	// Executors
	static QueryResult executeNameSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                      const Types::ReferenceArgument& rhs);
	static QueryResult executeWildcardSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                      const Types::ReferenceArgument& rhs);
	static QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs);
	static QueryResult executeSynonymSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
	                                         const Types::ReferenceArgument& rhs);
};

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_H
