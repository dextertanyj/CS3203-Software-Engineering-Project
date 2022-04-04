#ifndef SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

namespace QP::Executor::PatternAssignExecutor {

using namespace QP::Types;

// Trivial Executors
QueryResult executeTrivialNameWildcard(const StorageAdapter& store, const ReferenceArgument& name);
QueryResult executeTrivialSynonymOrWildcardWildcard(const StorageAdapter& store);
QueryResult executeTrivialNameExpression(const StorageAdapter& store, const ReferenceArgument& name, const ReferenceArgument& expression);
QueryResult executeTrivialSynonymOrWildcardExpression(const StorageAdapter& store, const ReferenceArgument& expression);

// Executors
QueryResult executeNameWildcard(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& name);
QueryResult executeWildcardWildcard(const StorageAdapter& store, const ReferenceArgument& assign);
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& synonym);
QueryResult executeNameExpression(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& name,
                                  const ReferenceArgument& expression);
QueryResult executeWildcardExpression(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& expression);
QueryResult executeSynonymExpression(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& synonym,
                                     const ReferenceArgument& expression);

// Executor Set Factories
ExecutorSet executorFactoryNameWildcard(const std::vector<ReferenceArgument>& args);
ExecutorSet executorFactoryNameExpression(const std::vector<ReferenceArgument>& args);
ExecutorSet executorFactoryWildcardWildcard(const std::vector<ReferenceArgument>& args);
ExecutorSet executorFactoryWildcardExpression(const std::vector<ReferenceArgument>& args);
ExecutorSet executorFactorySynonymWildcard(const std::vector<ReferenceArgument>& args);
ExecutorSet executorFactorySynonymExpression(const std::vector<ReferenceArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H
