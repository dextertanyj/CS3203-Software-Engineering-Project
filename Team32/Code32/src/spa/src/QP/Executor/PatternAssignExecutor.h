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
QueryResult executeTrivialNameWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& name);
QueryResult executeTrivialSynonymOrWildcardWildcard(QP::StorageAdapter& storage);
QueryResult executeTrivialNameExpression(QP::StorageAdapter& storage, const Types::ReferenceArgument& name,
                                         const Types::ReferenceArgument& expression);
QueryResult executeTrivialSynonymOrWildcardExpression(QP::StorageAdapter& storage, const Types::ReferenceArgument& expression);

// Executors
QueryResult executeNameWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& assign, const Types::ReferenceArgument& name);
QueryResult executeWildcardWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& assign);
QueryResult executeSynonymWildcard(QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                   const Types::ReferenceArgument& synonym);
QueryResult executeNameExpression(QP::StorageAdapter& storage, const Types::ReferenceArgument& assign, const Types::ReferenceArgument& name,
                                  const Types::ReferenceArgument& expression);
QueryResult executeWildcardExpression(QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                      const Types::ReferenceArgument& expression);
QueryResult executeSynonymExpression(QP::StorageAdapter& storage, const Types::ReferenceArgument& assign,
                                     const Types::ReferenceArgument& synonym, const Types::ReferenceArgument& expression);
};  // namespace QP::Executor::PatternAssignExecutor

#endif  // SPA_PATTERNASSIGNEXECUTOR_H
