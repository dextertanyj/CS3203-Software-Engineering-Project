#ifndef SPA_SRC_QP_EXECUTOR_EXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_EXECUTOR_H

#include "QP/Types.h"

namespace QP::Executor {
template <QP::Types::ClauseType T>
class ProcedureVariableExecutor;

namespace StatementExecutor {}

namespace StatementVariableExecutor {}

template <QP::Types::ClauseType T>
class ProcedureExecutor;

template <QP::Types::ClauseType T>
class PatternContainerStatementExecutor;

namespace WithExecutor {}

namespace AttributeExecutor {}

namespace PatternAssignExecutor {}
}

#endif  // SPA_SRC_QP_EXECUTOR_EXECUTOR_H
