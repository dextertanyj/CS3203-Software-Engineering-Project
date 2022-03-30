#ifndef SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

namespace QP::Executor::PatternAssignExecutor {

using namespace QP::Types;

// Trivial Executors
QueryResult executeTrivialNameWildcard(const StorageAdapter& storage, const ReferenceArgument& name);
QueryResult executeTrivialSynonymOrWildcardWildcard(const StorageAdapter& storage);
QueryResult executeTrivialNameExpression(const StorageAdapter& storage, const ReferenceArgument& name, const ReferenceArgument& expression);
QueryResult executeTrivialSynonymOrWildcardExpression(const StorageAdapter& storage, const ReferenceArgument& expression);

// Executors
QueryResult executeNameWildcard(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& name);
QueryResult executeWildcardWildcard(const StorageAdapter& storage, const ReferenceArgument& assign);
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& synonym);
QueryResult executeNameExpression(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& name,
                                  const ReferenceArgument& expression);
QueryResult executeWildcardExpression(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& expression);
QueryResult executeSynonymExpression(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& synonym,
                                     const ReferenceArgument& expression);

// Executor Set Factories
ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args);
ExecutorSet executorFactoryNameExpression(const vector<ReferenceArgument>& args);
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& args);
ExecutorSet executorFactoryWildcardExpression(const vector<ReferenceArgument>& args);
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args);
ExecutorSet executorFactorySynonymExpression(const vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H
