#ifndef SPA_SRC_SP_NODE_NODE_H
#define SPA_SRC_SP_NODE_NODE_H

#include "Common/TypeDefs.h"
#include "SP/SP.h"

namespace SP::Node {
class ExpressionNode;
class AssignmentNode;
class CallNode;
class IfNode;
template <StmtType T>
class PrintReadNode;
class ProcedureNode;
class ProgramNode;
class StatementListNode;
class StatementNode;
class VariableNode;
class WhileNode;
}

#endif  // SPA_SRC_SP_NODE_NODE_H
