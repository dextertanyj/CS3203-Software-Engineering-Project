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

void PKB::ControlFlowGraph::setNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second) {}

void PKB::ControlFlowGraph::removeNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second) {}

shared_ptr<PKB::NodeInterface> PKB::ControlFlowGraph::getNode(StmtRef ref) {
	auto iter = stmt_to_normal_node_store.find(ref);
	if (iter == stmt_to_normal_node_store.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return this->stmt_to_normal_node_store.find(ref)->second;
}

vector<shared_ptr<PKB::NodeInterface>> PKB::ControlFlowGraph::getPrevious(StmtRef ref) { return {}; }

vector<shared_ptr<PKB::NodeInterface>> PKB::ControlFlowGraph::getNext(StmtRef ref) { return {}; }

vector<shared_ptr<PKB::NodeInterface>> PKB::ControlFlowGraph::getPreviousTransitive(StmtRef ref) { return {}; }

vector<shared_ptr<PKB::NodeInterface>> PKB::ControlFlowGraph::getNextTransitive(StmtRef ref) { return {}; }

void PKB::ControlFlowGraph::optimize() {}

void PKB::ControlFlowGraph::populateUniqueIndex() {}
