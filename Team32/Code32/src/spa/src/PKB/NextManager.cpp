#include "NextManager.h"

PKB::NextManager::NextManager(ControlFlowGraph& control_flow_graph) { this->control_flow_graph = &control_flow_graph; }

void PKB::NextManager::setNext(StmtRef previous, StmtRef next) { control_flow_graph->setNext(previous, next); }

void PKB::NextManager::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	control_flow_graph->setIfNext(prev, then_next, else_next);
}

void PKB::NextManager::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	control_flow_graph->setIfExit(then_prev, else_prev, if_stmt_ref);
}

StmtInfoPtrSet PKB::NextManager::getNext(StmtRef first) { return control_flow_graph->getNextNodes(first); }

StmtInfoPtrSet PKB::NextManager::getNextStar(StmtRef node_ref) { return control_flow_graph->getNextStarNodes(node_ref); }

bool PKB::NextManager::checkNext(StmtRef first, StmtRef second) { return control_flow_graph->checkNext(first, second); }

bool PKB::NextManager::checkNextStar(StmtRef first, StmtRef second) { return control_flow_graph->checkNextStar(first, second); }

StmtInfoPtrSet PKB::NextManager::getPrevious(StmtRef second) { return control_flow_graph->getPreviousNodes(second); }

StmtInfoPtrSet PKB::NextManager::getPreviousStar(StmtRef node_ref) { return control_flow_graph->getPreviousStarNodes(node_ref); }
