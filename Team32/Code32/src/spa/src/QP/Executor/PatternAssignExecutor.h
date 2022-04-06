#ifndef SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H

#include "QP/ClauseArgument.h"
#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"

namespace QP::Executor::PatternAssignExecutor {

using namespace QP::Types;

// Trivial Executors
QueryResult executeTrivialNameWildcard(const StorageAdapter& store, const ClauseArgument& name);
QueryResult executeTrivialSynonymOrWildcardWildcard(const StorageAdapter& store);
QueryResult executeTrivialNameExpression(const StorageAdapter& store, const ClauseArgument& name, const ClauseArgument& expression);
QueryResult executeTrivialSynonymOrWildcardExpression(const StorageAdapter& store, const ClauseArgument& expression);

// Executors
QueryResult executeNameWildcard(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& name);
QueryResult executeWildcardWildcard(const StorageAdapter& store, const ClauseArgument& assign);
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& synonym);
QueryResult executeNameExpression(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& name,
                                  const ClauseArgument& expression);
QueryResult executeWildcardExpression(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& expression);
QueryResult executeSynonymExpression(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& synonym,
                                     const ClauseArgument& expression);

// Executor Set Factories
ExecutorSet executorFactoryNameWildcard(const std::vector<ClauseArgument>& args);
ExecutorSet executorFactoryNameExpression(const std::vector<ClauseArgument>& args);
ExecutorSet executorFactoryWildcardWildcard(const std::vector<ClauseArgument>& args);
ExecutorSet executorFactoryWildcardExpression(const std::vector<ClauseArgument>& args);
ExecutorSet executorFactorySynonymWildcard(const std::vector<ClauseArgument>& args);
ExecutorSet executorFactorySynonymExpression(const std::vector<ClauseArgument>& args);
};

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNASSIGNEXECUTOR_H
