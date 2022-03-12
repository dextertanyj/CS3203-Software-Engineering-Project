#ifndef SPA_EXECUTOR_H
#define SPA_EXECUTOR_H

#include "QP/QueryTypes.h"

namespace QP::Executor {
template <QP::Types::ClauseType T>
class ProcedureVariableExecutor;

template <QP::Types::ClauseType T>
class StatementExecutor;

template <QP::Types::ClauseType T>
class StatementVariableExecutor;

template <QP::Types::ClauseType T>
class ProcedureExecutor;

namespace PatternAssignExecutor {};
}  // namespace QP::Executor

#endif  // SPA_EXECUTOR_H
