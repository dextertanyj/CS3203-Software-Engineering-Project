#ifndef SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/Types.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

template <QP::Types::ClauseType T>
class QP::Executor::PatternContainerStatementExecutor {
public:
	// Trivial Executors
	static QueryResult executeTrivialName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& var);
	static QueryResult executeTrivialWildcardOrSynonym(const QP::StorageAdapter& storage);

	// Executors
	static QueryResult executeName(const QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                               const Types::ReferenceArgument& var);
	static QueryResult executeWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt);
	static QueryResult executeSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& stmt,
	                                  const Types::ReferenceArgument& var);
};

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_H
