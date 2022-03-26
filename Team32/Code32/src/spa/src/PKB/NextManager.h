#ifndef SPA_NEXTMANAGER_H
#define SPA_NEXTMANAGER_H

#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/NodeInterface.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/PKB.h"
#include "PKB/Types.h"

using namespace std;

class PKB::NextManager {
public:
	explicit NextManager(ControlFlowGraph& control_flow_graph);
	void setNext(StmtRef previous, StmtRef next);
	void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next);
	void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref);

	bool checkNext(StmtRef first, StmtRef second);
	bool checkNextStar(StmtRef first, StmtRef second);
	StmtInfoPtrSet getNext(StmtRef node_ref);
	StmtInfoPtrSet getNextStar(StmtRef node_ref);
	StmtInfoPtrSet getPrevious(StmtRef node_ref);
	StmtInfoPtrSet getPreviousStar(StmtRef node_ref);

private:
	ControlFlowGraph* control_flow_graph;
	void processBFSVisit(Types::BFSInfo& info, const shared_ptr<NodeInterface>& node,
	                     StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&));
};

#endif  // SPA_NEXTMANAGER_H
