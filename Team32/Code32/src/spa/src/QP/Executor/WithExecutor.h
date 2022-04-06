#ifndef SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_H

#include <functional>
#include <unordered_set>
#include <utility>

#include "QP/ClauseArgument.h"
#include "QP/QueryResult.h"
#include "QP/Types.h"

namespace QP::Executor::WithExecutor {

using namespace QP::Types;

// Trivial Executors
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeTrivialAttributeAttribute(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                                 WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                 WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeTrivialAttributeConstant(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                                WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeTrivialConstantAttribute(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                                WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeTrivialConstantConstant(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                               WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                               WithInternalExecutors<TAttribute, TRight> rhs_executors);

// Executors
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeAttributeAttribute(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                          WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                          WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeAttributeConstant(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                         WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                         WithInternalExecutors<TAttribute, TRight> rhs_executors);
template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult executeConstantAttribute(const QP::StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                                         WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                         WithInternalExecutors<TAttribute, TRight> rhs_executors);
}

#endif  // SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_H
