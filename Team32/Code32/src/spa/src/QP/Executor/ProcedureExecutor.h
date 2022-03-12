#ifndef SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::ProcedureExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameName(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller,
	                                          const Types::ReferenceArgument& callee);
	static QueryResult executeTrivialNameWildcardOrSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller);
	static QueryResult executeTrivialWildcardOrSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& callee);
	static QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(QP::StorageAdapter& storage);
	static QueryResult executeTrivialSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller,
	                                                const Types::ReferenceArgument& callee);

	// Executors
	static QueryResult executeNameSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller,
	                                      const Types::ReferenceArgument& callee);
	static QueryResult executeWildcardSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& callee);
	static QueryResult executeSynonymName(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller,
	                                      const Types::ReferenceArgument& callee);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& storage, const Types::ReferenceArgument& caller,
	                                         const Types::ReferenceArgument& callee);
};

#endif  // SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_H
