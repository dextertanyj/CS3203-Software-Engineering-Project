#include "ControlFlowGraph.h"

#include "DummyNode.h"
#include "IfNode.h"
#include "NonConditionalNode.h"
#include "WhileNode.h"

void PKB::ControlFlowGraph::createNode(shared_ptr<StmtInfo> stmt_info) {
	if (stmt_to_normal_node_store.find(stmt_info->getIdentifier()) != stmt_to_normal_node_store.end()) {
		throw invalid_argument("Node has already been created.");
	}
	// Check node type and create respective node.
	if (stmt_info->getType() == StmtType::IfStmt) {
		shared_ptr<PKB::IfNode> to_insert = make_shared<PKB::IfNode>(PKB::IfNode(stmt_info));
		this->stmt_to_normal_node_store.insert({stmt_info->getIdentifier(), to_insert});

	} else if (stmt_info->getType() == StmtType::WhileStmt) {
		shared_ptr<PKB::WhileNode> to_insert = make_shared<PKB::WhileNode>(PKB::WhileNode(stmt_info));
		this->stmt_to_normal_node_store.insert({stmt_info->getIdentifier(), to_insert});
	} else {
		shared_ptr<PKB::NonConditionalNode> to_insert = make_shared<PKB::NonConditionalNode>(PKB::NonConditionalNode(stmt_info));
		this->stmt_to_normal_node_store.insert({stmt_info->getIdentifier(), to_insert});
	}
}

bool PKB::ControlFlowGraph::checkNext(StmtRef prev, StmtRef next) {
	auto prev_node_iter = this->stmt_to_normal_node_store.find(prev);
	auto next_node_iter = this->stmt_to_normal_node_store.find(next);
	if (prev_node_iter == stmt_to_normal_node_store.end() || next_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	StmtRefSet next_nodes_of_prev = this->getNextNodes(prev);
	return find(next_nodes_of_prev.begin(), next_nodes_of_prev.end(), next) != next_nodes_of_prev.end();
}

shared_ptr<PKB::NodeInterface> PKB::ControlFlowGraph::getNode(StmtRef ref) {
	auto iter = stmt_to_normal_node_store.find(ref);
	if (iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return this->stmt_to_normal_node_store.find(ref)->second;
}

StmtRefSet PKB::ControlFlowGraph::getPreviousNodes(StmtRef ref) {
	shared_ptr<PKB::NodeInterface> curr_node = this->getNode(ref);
	StmtRefSet prev_nodes;
	for (auto node : curr_node->getPreviousNodes()) {
		// If previous node is a dummy node, need to get the previous nodes of the dummy node.
		if (node->getNodeType() == NodeType::Dummy) {
			StmtRefSet prev_of_dummy = collectPreviousOfDummy(node);
			prev_nodes.insert(prev_of_dummy.begin(), prev_of_dummy.end());
		} else {
			prev_nodes.insert(node->getNodeRef());
		}
	}
	return prev_nodes;
}

StmtRefSet PKB::ControlFlowGraph::getNextNodes(StmtRef ref) {
	shared_ptr<PKB::NodeInterface> curr_node = this->getNode(ref);
	StmtRefSet next_nodes;
	for (auto node : curr_node->getNextNodes()) {
		if (node->getNodeType() == NodeType::Dummy) {
			shared_ptr<PKB::NodeInterface> lowest_dummy_node = findLowestDummy(node);
			set<shared_ptr<PKB::NodeInterface>> next_of_dummy = lowest_dummy_node->getNextNodes();
			for (auto next_node_of_dummy : next_of_dummy) {
				next_nodes.insert(next_node_of_dummy->getNodeRef());
			}
		} else {
			next_nodes.insert(node->getNodeRef());
		}
	}
	return next_nodes;
}

void PKB::ControlFlowGraph::setNext(StmtRef prev, StmtRef next) {
	auto prev_node_iter = this->stmt_to_normal_node_store.find(prev);
	auto next_node_iter = this->stmt_to_normal_node_store.find(next);
	if (prev_node_iter == stmt_to_normal_node_store.end() || next_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	if (prev == next) {
		throw invalid_argument("Cannot set a node's direct next to itself.");
	}
	prev_node_iter->second->insertNext(next_node_iter->second);
	next_node_iter->second->insertPrevious(prev_node_iter->second);
}

void PKB::ControlFlowGraph::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	auto prev_node_iter = this->stmt_to_normal_node_store.find(prev);
	auto then_next_node_iter = this->stmt_to_normal_node_store.find(then_next);
	auto else_next_node_iter = this->stmt_to_normal_node_store.find(else_next);
	if (prev_node_iter == stmt_to_normal_node_store.end() || then_next_node_iter == stmt_to_normal_node_store.end() ||
	    else_next_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	if (prev >= then_next || prev >= else_next || then_next >= else_next) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	if (prev_node_iter->second->getNodeType() != NodeType::If) {
		throw invalid_argument("First argument must refer to an if statement.");
	}
	shared_ptr<PKB::IfNode> if_ctrl_node = dynamic_pointer_cast<IfNode>(prev_node_iter->second);
	if_ctrl_node->insertIfNext(then_next_node_iter->second, else_next_node_iter->second, if_ctrl_node);
}

void PKB::ControlFlowGraph::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	auto then_prev_node_iter = this->stmt_to_normal_node_store.find(then_prev);
	auto else_prev_node_iter = this->stmt_to_normal_node_store.find(else_prev);
	auto if_ctrl_node_iter = this->stmt_to_normal_node_store.find(if_stmt_ref);
	if (then_prev_node_iter == stmt_to_normal_node_store.end() || else_prev_node_iter == stmt_to_normal_node_store.end() ||
	    if_ctrl_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	if (if_stmt_ref >= then_prev || if_stmt_ref >= else_prev || then_prev >= else_prev) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	if (if_ctrl_node_iter->second->getNodeType() != NodeType::If) {
		throw invalid_argument("Third argument must refer to an if control statement.");
	}
	shared_ptr<PKB::IfNode> if_node = dynamic_pointer_cast<IfNode>(if_ctrl_node_iter->second);
	if_node->insertIfExit(then_prev_node_iter->second, else_prev_node_iter->second);
}

shared_ptr<PKB::NodeInterface> PKB::ControlFlowGraph::findLowestDummy(shared_ptr<PKB::NodeInterface> dummy_node) {
	shared_ptr<PKB::NodeInterface> result = dummy_node;
	while (result->getNextNodes().size() == 1 && result->getNextNodes().begin()->get()->getNodeType() == NodeType::Dummy) {
		result = *result->getNextNodes().begin();
	}
	return result;
}

StmtRefSet PKB::ControlFlowGraph::collectPreviousOfDummy(shared_ptr<PKB::NodeInterface> dummy_node) {
	StmtRefSet collection;
	for (auto prev_node : dummy_node->getPreviousNodes()) {
		if (prev_node->getNodeType() == NodeType::Dummy) {
			StmtRefSet sub_collection = collectPreviousOfDummy(prev_node);
			collection.insert(sub_collection.begin(), sub_collection.end());
		} else {
			collection.insert(prev_node->getNodeRef());
		}
	}
	return collection;
}

void PKB::ControlFlowGraph::clear() { this->stmt_to_normal_node_store.clear(); }

// TODO: Next* TBD.
void PKB::ControlFlowGraph::optimize() {}
void PKB::ControlFlowGraph::populateGraphIndex() {}
bool PKB::ControlFlowGraph::checkNextStar(StmtRef i, StmtRef i1) { return false; }
