#include "AffectsManager.h"

PKB::AffectsManager::AffectsManager(ControlFlowGraph& control_flow_graph, const SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
                                    const SVRelationStore<PKB::UsesSRelation>& uses_store) {
	this->control_flow_graph = &control_flow_graph;
	this->uses_store = uses_store;
	this->modifies_store = modifies_store;
}

bool PKB::AffectsManager::checkAffects(StmtRef first, StmtRef second) {
	StmtInfoPtrSet affected_nodes = getAffects(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [second](shared_ptr<StmtInfo> info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffects(StmtRef first) {
	shared_ptr<PKB::StatementNode> node = this->control_flow_graph->getNode(first);
	if (node->getStmtInfo()->getType() != StmtType::Assign) {
		throw invalid_argument("Affects statement must be an assign statement.");
	}

	VarRef variable = *(modifies_store.getByStmt(first).begin());
	Types::DFSInfo info = {node, variable, {}, {}, {}};
	info.visited_set.insert(node);
	for (auto neighbour : node->getNextNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		shared_ptr<PKB::NodeInterface> curr_node = info.node_stack.top();
		info.node_stack.pop();

		if (info.visited_set.find(curr_node) != info.visited_set.end()) {
			continue;
		}

		if (node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet real_nodes = control_flow_graph->collectNextOfDummy(node);
			for (auto real_node : real_nodes) {
				info.node_stack.push(control_flow_graph->stmt_to_normal_node_store.at(real_node->getIdentifier()));
			}
			continue;
		}

		shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(curr_node);
		if (uses_store.check(stmt_node->getStmtInfo()->getIdentifier(), info.variable)) {
			info.nodes.insert(stmt_node->getStmtInfo());
		} else {
			for (auto neighbour : curr_node->getNextNodes()) {
				info.node_stack.push(neighbour);
			}
		}
		info.visited_set.insert(curr_node);
	}
	return info.nodes;
}

StmtInfoPtrSet PKB::AffectsManager::getAffected(StmtRef second) {
	shared_ptr<PKB::StatementNode> node = this->control_flow_graph->getNode(second);
	if (node->getStmtInfo()->getType() != StmtType::Assign) {
		throw invalid_argument("Affects statement must be an assign statement.");
	}

	VarRefSet variables = uses_store.getByStmt(second);
	StmtInfoPtrSet affected_set;
	for (auto variable : variables) {
		StmtInfoPtrSet affected = processAffected(node, variable);
		affected_set.insert(affected.begin(), affected.end());
	}
	return affected_set;
}

StmtInfoPtrSet PKB::AffectsManager::processAffected(shared_ptr<PKB::StatementNode> node, VarRef variable) {
	Types::DFSInfo info = {node, variable, {}, {}, {}};
	info.visited_set.insert(node);
	for (auto neighbour : node->getPreviousNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		shared_ptr<PKB::NodeInterface> curr_node = info.node_stack.top();
		info.node_stack.pop();

		if (info.visited_set.find(curr_node) != info.visited_set.end()) {
			continue;
		}

		if (node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet real_nodes = control_flow_graph->collectPreviousOfDummy(node);
			for (auto real_node : real_nodes) {
				info.node_stack.push(control_flow_graph->stmt_to_normal_node_store.at(real_node->getIdentifier()));
			}
			continue;
		}

		shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(curr_node);
		if (modifies_store.check(stmt_node->getStmtInfo()->getIdentifier(), info.variable)) {
			info.nodes.insert(stmt_node->getStmtInfo());
		} else {
			for (auto neighbour : curr_node->getPreviousNodes()) {
				info.node_stack.push(neighbour);
			}
		}
		info.visited_set.insert(curr_node);
	}
	return info.nodes;
}
void PKB::AffectsManager::processDFS(Types::DFSInfo& info, const shared_ptr<NodeInterface>& node,
                                     StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface>&)) {}
