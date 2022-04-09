#ifndef SPA_SRC_PKB_CFG_CFG_H
#define SPA_SRC_PKB_CFG_CFG_H

#include "PKB/PKB.h"

namespace PKB::CFG {
class NodeInterface;
class StatementNode;  // NOLINT(bugprone-forward-declaration-namespace)
class IfNode;         // NOLINT(bugprone-forward-declaration-namespace)
class WhileNode;      // NOLINT(bugprone-forward-declaration-namespace)
class DummyNode;
class ControlFlowGraph;
}

#endif  // SPA_SRC_PKB_CFG_CFG_H
