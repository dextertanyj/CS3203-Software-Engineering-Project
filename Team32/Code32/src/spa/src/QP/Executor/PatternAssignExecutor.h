//
// Created by Dexter on 12/3/22.
//

#ifndef SPA_PATTERNASSIGNEXECUTOR_H
#define SPA_PATTERNASSIGNEXECUTOR_H

#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

namespace QP::Executor::PatternAssignExecutor {
// Trivial Executors
QueryResult executeTrivialNameWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& name);
QueryResult executeTrivialSynonymOrWildcardWildcard(const QP::StorageAdapter& storage);
QueryResult executeTrivialNameExpression(const QP::StorageAdapter& storage, const Types::ReferenceArgument& name,
                                         const Types::ReferenceArgument& expression);
QueryResult executeTrivialSynonymOrWildcardExpression(const QP::StorageAdapter& storage, const Types::ReferenceArgument& expression);

// Executors
QueryResult executeNameWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                const Types::ReferenceArgument& name);
QueryResult executeWildcardWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& assign);
QueryResult executeSynonymWildcard(const QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                   const Types::ReferenceArgument& synonym);
QueryResult executeNameExpression(const QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                  const Types::ReferenceArgument& name, const Types::ReferenceArgument& expression);
QueryResult executeWildcardExpression(const QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                      const Types::ReferenceArgument& expression);
QueryResult executeSynonymExpression(const QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                     const Types::ReferenceArgument& synonym, const Types::ReferenceArgument& expression);
};  // namespace QP::Executor::PatternAssignExecutor

#endif  // SPA_PATTERNASSIGNEXECUTOR_H
