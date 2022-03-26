#include "NextManager.h"

PKB::NextManager::NextManager(ControlFlowGraph &control_flow_graph) : control_flow_graph(control_flow_graph) {}

void PKB::NextManager::setNext(StmtRef previous, StmtRef next) {
	if (previous == next) {
		throw invalid_argument("Cannot set a node's direct next to itself.");
	}
	try {
		auto prev_node = control_flow_graph.getNode(previous);
		auto next_node = control_flow_graph.getNode(next);
		prev_node->setConnection(next_node);
	} catch (invalid_argument e) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
}

void PKB::NextManager::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	if (prev >= then_next || prev >= else_next || then_next >= else_next) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	try {
		auto prev_node = control_flow_graph.getNode(prev);
		auto then_next_node = control_flow_graph.getNode(then_next);
		auto else_next_node = control_flow_graph.getNode(else_next);
		if (prev_node->getNodeType() != NodeType::If) {
			throw invalid_argument("First argument must refer to an if statement.");
		}
		shared_ptr<PKB::IfNode> if_ctrl_node = dynamic_pointer_cast<PKB::IfNode>(prev_node);
		if_ctrl_node->insertIfNext(then_next_node, else_next_node);
	} catch (invalid_argument e) {
		throw e.what();
	}
}

void PKB::NextManager::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	if (if_stmt_ref >= then_prev || if_stmt_ref >= else_prev || then_prev >= else_prev) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	try {
		auto then_prev_node = control_flow_graph.getNode(then_prev);
		auto else_prev_node = control_flow_graph.getNode(else_prev);
		auto if_ctrl_node = control_flow_graph.getNode(if_stmt_ref);
		if (if_ctrl_node->getNodeType() != NodeType::If) {
			throw invalid_argument("Third argument must refer to an if control statement.");
		}
		shared_ptr<PKB::IfNode> if_node = dynamic_pointer_cast<PKB::IfNode>(if_ctrl_node);
		if_node->insertIfExit(then_prev_node, else_prev_node);
	} catch (invalid_argument e) {
		throw e.what();
	}
}

bool PKB::NextManager::checkNext(StmtRef first, StmtRef second) {
	try {
		auto prev_node = control_flow_graph.getNode(first);
		auto next_node = control_flow_graph.getNode(second);
		StmtInfoPtrSet next_nodes_of_prev = this->getNext(first);
		return any_of(next_nodes_of_prev.begin(), next_nodes_of_prev.end(),
		              [second](const shared_ptr<StmtInfo> &next_info) { return next_info->getIdentifier() == second; });
	} catch (invalid_argument) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
}

StmtInfoPtrSet PKB::NextManager::getNextStar(StmtRef node_ref) {
	Types::BFSInfo info = {control_flow_graph.getNode(node_ref), {}, {}, {}};
	info.node_queue.push(info.start_node);
	while (!info.node_queue.empty()) {
		shared_ptr<PKB::NodeInterface> curr_node = info.node_queue.front();
		info.node_queue.pop();
		unordered_set<shared_ptr<PKB::NodeInterface>> curr_next_nodes = curr_node->getNextNodes();
		for (const auto &node : curr_next_nodes) {
			processBFSVisit(info, node, &ControlFlowGraph::collectNextOfDummy);
		}
		info.visited_set.insert(curr_node);
	}
	return info.nodes;
}

StmtInfoPtrSet PKB::NextManager::getNext(StmtRef node_ref) {
	shared_ptr<PKB::StatementNode> curr_node = control_flow_graph.getNode(node_ref);
	StmtInfoPtrSet next_nodes;
	for (const auto &node : curr_node->getNextNodes()) {
		if (node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet next_of_dummy = control_flow_graph.collectNextOfDummy(node);
			next_nodes.insert(next_of_dummy.begin(), next_of_dummy.end());
		} else {
			shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(node);
			next_nodes.insert(stmt_node->getStmtInfo());
		}
	}
	return next_nodes;
}

bool PKB::NextManager::checkNextStar(StmtRef first, StmtRef second) {
	StmtInfoPtrSet nexts = getNextStar(first);
	return any_of(nexts.begin(), nexts.end(), [&second](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::NextManager::getPrevious(StmtRef node_ref) {
	shared_ptr<PKB::StatementNode> curr_node = this->control_flow_graph.getNode(node_ref);
	StmtInfoPtrSet prev_nodes;
	for (const auto &node : curr_node->getPreviousNodes()) {
		// If previous node is a dummy node, need to get the previous nodes of the dummy node.
		if (node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet prev_of_dummy = control_flow_graph.collectPreviousOfDummy(node);
			prev_nodes.insert(prev_of_dummy.begin(), prev_of_dummy.end());
		} else {
			shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<StatementNode>(node);
			prev_nodes.insert(stmt_node->getStmtInfo());
		}
	}
	return prev_nodes;
}

StmtInfoPtrSet PKB::NextManager::getPreviousStar(StmtRef node_ref) {
	Types::BFSInfo info = {control_flow_graph.getNode(node_ref), {}, {}, {}};
	info.node_queue.push(info.start_node);
	while (!info.node_queue.empty()) {
		shared_ptr<PKB::NodeInterface> curr_node = info.node_queue.front();
		info.node_queue.pop();
		unordered_set<shared_ptr<PKB::NodeInterface>> curr_prev_nodes = curr_node->getPreviousNodes();
		for (const auto &node : curr_prev_nodes) {
			processBFSVisit(info, node, &ControlFlowGraph::collectPreviousOfDummy);
		}
		info.visited_set.insert(curr_node);
	}
	return info.nodes;
}

void PKB::NextManager::processBFSVisit(Types::BFSInfo &info, const shared_ptr<PKB::NodeInterface> &node,
                                       StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	if (node == info.start_node) {
		shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(node);
		info.nodes.insert(stmt_node->getStmtInfo());
	}
	if (info.visited_set.find(node) != info.visited_set.end()) {
		return;
	}
	if (node->getNodeType() == NodeType::Dummy) {
		StmtInfoPtrSet real_nodes = (control_flow_graph.*collector)(node);
		info.nodes.insert(real_nodes.begin(), real_nodes.end());
		for (const auto &real_node : real_nodes) {
			info.node_queue.push(control_flow_graph.getNode(real_node->getIdentifier()));
		}
	} else {
		shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(node);
		info.nodes.insert(stmt_node->getStmtInfo());
		info.node_queue.push(node);
	}
}
