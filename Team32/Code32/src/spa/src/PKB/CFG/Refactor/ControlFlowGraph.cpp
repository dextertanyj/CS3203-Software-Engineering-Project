#include "ControlFlowGraph.h"

#include "DummyNode.h"
#include "IfNode.h"
#include "NonConditionalNode.h"
#include "WhileNode.h"

void PKB::ControlFlowGraph::createNode(shared_ptr<StmtInfo> stmt_info) {
	// Check node type and create respective node.
	if (stmt_info->getType() == StmtType::IfStmt) {
		shared_ptr<PKB::IfNode> to_insert = make_shared<PKB::IfNode>(PKB::IfNode(stmt_info));
		this->createDummyNode(to_insert);
		this->stmt_to_normal_node_store.insert({stmt_info->getIdentifier(), to_insert});

	} else if (stmt_info->getType() == StmtType::WhileStmt) {
		shared_ptr<PKB::WhileNode> to_insert = make_shared<PKB::WhileNode>(PKB::WhileNode(stmt_info));
		this->stmt_to_normal_node_store.insert({stmt_info->getIdentifier(), to_insert});
	} else {
		shared_ptr<PKB::NonConditionalNode> to_insert = make_shared<PKB::NonConditionalNode>(PKB::NonConditionalNode(stmt_info));
		this->stmt_to_normal_node_store.insert({stmt_info->getIdentifier(), to_insert});
	}
}

void PKB::ControlFlowGraph::createDummyNode(shared_ptr<PKB::IfNode> if_node) {
	PKB::DummyNode dummy = PKB::DummyNode(if_node->getNodeRef());
	shared_ptr<PKB::DummyNode> dummy_ptr = make_shared<PKB::DummyNode>(dummy);
	this->stmt_to_dummy_node_store.insert({if_node->getNodeRef(), dummy_ptr});
	if_node->setDummyNode(dummy_ptr);
}

shared_ptr<PKB::NodeInterface> PKB::ControlFlowGraph::getNode(StmtRef ref) {
	auto iter = stmt_to_normal_node_store.find(ref);
	if (iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return this->stmt_to_normal_node_store.find(ref)->second;
}

vector<shared_ptr<PKB::NodeInterface>> PKB::ControlFlowGraph::getPreviousNodes(StmtRef ref) {
	auto iter = this->stmt_to_normal_node_store.find(ref);
	if (iter == this->stmt_to_normal_node_store.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return iter->second.get()->getPreviousNodes();
}

vector<shared_ptr<PKB::NodeInterface>> PKB::ControlFlowGraph::getNextNodes(StmtRef ref) {
	auto iter = this->stmt_to_normal_node_store.find(ref);
	if (iter == this->stmt_to_normal_node_store.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return iter->second.get()->getNextNodes();
}

void PKB::ControlFlowGraph::setNext(StmtRef prev, StmtRef next) {
	auto prev_node_iter = this->stmt_to_normal_node_store.find(prev);
	auto next_node_iter = this->stmt_to_normal_node_store.find(next);
	if (prev_node_iter == stmt_to_normal_node_store.end() || next_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	shared_ptr<PKB::NodeInterface> prev_node_ptr = prev_node_iter->second;
	shared_ptr<PKB::NodeInterface> next_node_ptr = next_node_iter->second;
	if (prev_node_ptr->getNodeType() == "if") {
		// Dummy node of the if node connects to the next node.
		shared_ptr<PKB::NodeInterface> dummy_node = prev_node_ptr->getDummyNode();
		this->set(dummy_node, next_node_ptr);
	} else {
		this->set(prev_node_ptr, next_node_ptr);
	}
}

void PKB::ControlFlowGraph::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	auto prev_node_iter = this->stmt_to_normal_node_store.find(prev);
	auto then_next_node_iter = this->stmt_to_normal_node_store.find(then_next);
	auto else_next_node_iter = this->stmt_to_normal_node_store.find(else_next);
	if (prev_node_iter == stmt_to_normal_node_store.end() || then_next_node_iter == stmt_to_normal_node_store.end() ||
	    else_next_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	shared_ptr<PKB::NodeInterface> if_ctrl_node = prev_node_iter->second;
	shared_ptr<PKB::NodeInterface> then_next_node = then_next_node_iter->second;
	shared_ptr<PKB::NodeInterface> else_next_node = else_next_node_iter->second;
	set(if_ctrl_node, then_next_node);
	set(if_ctrl_node, else_next_node);
}

void PKB::ControlFlowGraph::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	auto then_prev_node_iter = this->stmt_to_normal_node_store.find(then_prev);
	auto else_prev_node_iter = this->stmt_to_normal_node_store.find(else_prev);
	auto if_ctrl_node_iter = this->stmt_to_normal_node_store.find(if_stmt_ref);
	if (then_prev_node_iter == stmt_to_normal_node_store.end() || else_prev_node_iter == stmt_to_normal_node_store.end() ||
	    if_ctrl_node_iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("One of the provided references is not an existing node.");
	}
	shared_ptr<PKB::NodeInterface> then_prev_node = then_prev_node_iter->second;
	shared_ptr<PKB::NodeInterface> else_prev_node = else_prev_node_iter->second;
	// set ending nodes to the dummy node of the if control node.
	shared_ptr<PKB::NodeInterface> dummy_node = if_ctrl_node_iter->second->getDummyNode();
	set(then_prev_node, dummy_node);
	set(else_prev_node, dummy_node);
}

void PKB::ControlFlowGraph::set(shared_ptr<PKB::NodeInterface> prev, shared_ptr<PKB::NodeInterface> next) {
	prev->insertNext(next);
	next->insertPrevious(prev);
}

void PKB::ControlFlowGraph::removeNextConnection(StmtRef first, StmtRef second) {}

void PKB::ControlFlowGraph::optimize() {}

void PKB::ControlFlowGraph::populateUniqueIndex() {}
