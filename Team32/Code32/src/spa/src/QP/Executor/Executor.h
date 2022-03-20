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

template <QP::Types::ClauseType T>
class PatternContainerStatementExecutor;

namespace PatternAssignExecutor {};
}

#endif  // SPA_EXECUTOR_H
