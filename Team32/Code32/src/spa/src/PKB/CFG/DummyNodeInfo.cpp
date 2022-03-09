#include "DummyNodeInfo.h"

bool PKB::DummyNodeInfo::isDummy() const { return true; }

// Placeholder - use INT_MAX as stmtref and print as stmt type for dummy node.
PKB::DummyNodeInfo::DummyNodeInfo() : PKB::NodeInfo(INT_MAX, StmtType::Print) {}
