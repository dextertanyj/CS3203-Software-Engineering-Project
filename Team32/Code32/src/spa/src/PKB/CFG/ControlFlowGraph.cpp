#include "PKB/CFG/ControlFlowGraph.h"

#include <cassert>

#include "PKB/CFG/DummyNode.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/NonConditionalNode.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/CFG/WhileNode.h"

void PKB::ControlFlowGraph::createNode(shared_ptr<StmtInfo> stmt_info) {
	if (statement_node_map.find(stmt_info->getIdentifier()) != statement_node_map.end()) {
		throw invalid_argument("Node has already been created.");
	}
	// Check node type and create respective node.
	if (stmt_info->getType() == StmtType::IfStmt) {
		shared_ptr<PKB::IfNode> to_insert = make_shared<PKB::IfNode>(stmt_info);
		this->statement_node_map.insert({stmt_info->getIdentifier(), to_insert});

	} else if (stmt_info->getType() == StmtType::WhileStmt) {
		shared_ptr<PKB::WhileNode> to_insert = make_shared<PKB::WhileNode>(stmt_info);
		this->statement_node_map.insert({stmt_info->getIdentifier(), to_insert});
	} else {
		shared_ptr<PKB::NonConditionalNode> to_insert = make_shared<PKB::NonConditionalNode>(stmt_info);
		this->statement_node_map.insert({stmt_info->getIdentifier(), to_insert});
	}
}

shared_ptr<PKB::StatementNode> PKB::ControlFlowGraph::getNode(StmtRef ref) {
	auto iter = statement_node_map.find(ref);
	if (iter == statement_node_map.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return this->statement_node_map.find(ref)->second;
}

shared_ptr<PKB::NodeInterface> PKB::ControlFlowGraph::getStart(size_t graph_index) {
	assert(start_map.find(graph_index) != start_map.end());
	return start_map.find(graph_index)->second;
}

shared_ptr<PKB::NodeInterface> PKB::ControlFlowGraph::getEnd(size_t graph_index) {
	assert(end_map.find(graph_index) != end_map.end());
	return end_map.find(graph_index)->second;
}

StmtInfoPtrSet PKB::ControlFlowGraph::collectNextOfDummy(const shared_ptr<PKB::NodeInterface>& dummy_node) {
	shared_ptr<PKB::NodeInterface> result = dummy_node;
	while (result->getNextNodes().size() == 1 && result->getNextNodes().begin()->get()->getNodeType() == NodeType::Dummy) {
		result = *result->getNextNodes().begin();
	}
	unordered_set<shared_ptr<PKB::NodeInterface>> next_node_of_dummy = result->getNextNodes();
	if (next_node_of_dummy.size() > 1) {
		throw logic_error("There should only be one next node of dummy.");
	}
	if (next_node_of_dummy.empty()) {
		return {};
	}
	shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(*(next_node_of_dummy.begin()));
	return {stmt_node->getStmtInfo()};
}

StmtInfoPtrSet PKB::ControlFlowGraph::collectPreviousOfDummy(const shared_ptr<PKB::NodeInterface>& dummy_node) {
	StmtInfoPtrSet collection;
	for (const auto& prev_node : dummy_node->getPreviousNodes()) {
		if (prev_node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet sub_collection = collectPreviousOfDummy(prev_node);
			collection.insert(sub_collection.begin(), sub_collection.end());
		} else {
			shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(prev_node);
			collection.insert(stmt_node->getStmtInfo());
		}
	}
	return collection;
}

void PKB::ControlFlowGraph::optimize() {
	size_t graph_index = 0;
	for (const auto& node : statement_node_map) {
		if (node.second->getPreviousNodes().empty()) {
			start_map.insert({graph_index, node.second});
			unordered_set<shared_ptr<NodeInterface>> visited;
			processGraphNode(node.second, graph_index, visited);
			graph_index++;
		}
	}
}

void PKB::ControlFlowGraph::processGraphNode(shared_ptr<NodeInterface> node, size_t graph_index,
                                             unordered_set<shared_ptr<NodeInterface>>& visited) {
	node->setGraphIndex(graph_index);
	visited.insert(node);
	if (node->getNextNodes().empty()) {
		end_map.insert({graph_index, node});
		return;
	}
	for (const auto& next : node->getNextNodes()) {
		if (visited.find(next) != visited.end()) {
			continue;
		}
		processGraphNode(next, graph_index, visited);
	}
}

void PKB::ControlFlowGraph::clear() {
	statement_node_map.clear();
	start_map.clear();
	end_map.clear();
}
