#ifndef SPA_AFFECTSMANAGER_H
#define SPA_AFFECTSMANAGER_H

#include "PKB/PKB.h"
#include "PKB/CFG/ControlFlowGraph.h"

class PKB::AffectsManager {
public:
	explicit AffectsManager(ControlFlowGraph& control_flow_graph);
private:
	ControlFlowGraph* control_flow_graph;
};

#endif  // SPA_AFFECTSMANAGER_H
