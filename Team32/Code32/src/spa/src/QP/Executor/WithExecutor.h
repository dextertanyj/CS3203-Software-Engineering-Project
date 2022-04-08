#ifndef SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_H

#include "QP/ClauseArgument.h"
#include "QP/Executor/Executor.h"
#include "QP/QueryResult.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::WithExecutor {

using namespace QP::Types;

// Trivial Executors
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeTrivialAttributeAttribute(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                             WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                             WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeTrivialAttributeConstant(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                            WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                            WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeTrivialConstantAttribute(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                            WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                            WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeTrivialConstantConstant(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                           WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                           WithInternalExecutors<TAttribute, TRight> rhs_executors);

// Executors
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeAttributeAttribute(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                      WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                      WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeAttributeConstant(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                     WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                     WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QueryResult executeConstantAttribute(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                     WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                     WithInternalExecutors<TAttribute, TRight> rhs_executors);
}

#endif  // SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_H
