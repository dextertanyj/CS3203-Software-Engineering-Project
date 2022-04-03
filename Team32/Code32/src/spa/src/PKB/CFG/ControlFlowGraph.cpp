#include "PKB/CFG/ControlFlowGraph.h"

#include <cassert>

#include "PKB/CFG/DummyNode.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/NonConditionalNode.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/CFG/WhileNode.h"

void PKB::ControlFlowGraph::setNext(StmtRef previous, StmtRef next) {
	if (previous == next) {
		throw invalid_argument("Cannot set a node's direct next to itself.");
	}
	try {
		auto prev_node = getNode(previous);
		auto next_node = getNode(next);
		prev_node->setConnection(next_node);
	} catch (const invalid_argument& e) {
		throw e;
	}
}

void PKB::ControlFlowGraph::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	if (prev >= then_next || prev >= else_next || then_next >= else_next) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	try {
		auto prev_node = getNode(prev);
		auto then_next_node = getNode(then_next);
		auto else_next_node = getNode(else_next);
		if (prev_node->getNodeType() != NodeType::If) {
			throw invalid_argument("First argument must refer to an if statement.");
		}
		shared_ptr<PKB::IfNode> if_ctrl_node = dynamic_pointer_cast<PKB::IfNode>(prev_node);
		if_ctrl_node->insertIfNext(then_next_node, else_next_node);
	} catch (const invalid_argument& e) {
		throw e;
	}
}

void PKB::ControlFlowGraph::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	if (if_stmt_ref >= then_prev || if_stmt_ref >= else_prev || then_prev >= else_prev) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	try {
		auto then_prev_node = getNode(then_prev);
		auto else_prev_node = getNode(else_prev);
		auto if_ctrl_node = getNode(if_stmt_ref);
		if (if_ctrl_node->getNodeType() != NodeType::If) {
			throw invalid_argument("Third argument must refer to an if control statement.");
		}
		shared_ptr<PKB::IfNode> if_node = dynamic_pointer_cast<PKB::IfNode>(if_ctrl_node);
		if_node->insertIfExit(then_prev_node, else_prev_node);
	} catch (const invalid_argument& e) {
		throw e;
	}
}

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

bool PKB::ControlFlowGraph::contains(StmtRef index) const {
	return statement_node_map.find(index) != statement_node_map.end();
}

shared_ptr<PKB::StatementNode> PKB::ControlFlowGraph::getNode(StmtRef ref) const {
	auto iter = statement_node_map.find(ref);
	if (iter == statement_node_map.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return this->statement_node_map.find(ref)->second;
}

shared_ptr<StmtInfo> PKB::ControlFlowGraph::getStatementInfo(StmtRef index) const {
	auto node = getNode(index);
	assert(node != nullptr);
	return node->getStmtInfo();
}

StmtType PKB::ControlFlowGraph::getType(StmtRef ref) const {
	auto node = getNode(ref);
	assert(node != nullptr);
	return node->getStmtInfo()->getType();
}

size_t PKB::ControlFlowGraph::getGraphIndex(StmtRef ref) const {
	auto iter = statement_node_map.find(ref);
	if (iter == statement_node_map.end()) {
		throw invalid_argument("This node does not exist in the store.");
	}
	return iter->second->getGraphIndex();
}

StmtRef PKB::ControlFlowGraph::getFirstIndex(size_t graph_index) const {
	assert(start_map.find(graph_index) != start_map.end());
	return first_index_map.at(graph_index);
}

StmtRef PKB::ControlFlowGraph::getLastIndex(size_t graph_index) const {
	assert(start_map.find(graph_index) != start_map.end());
	return last_index_map.at(graph_index);
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
		if (node.second->getPreviousNodes().empty() ||
		    (node.second->getNodeType() == NodeType::While && node.second->getPreviousNodes().size() == 1)) {
			StmtRef last = node.second->getNodeRef();
			start_map.insert({graph_index, node.second});
			unordered_set<shared_ptr<NodeInterface>> visited;
			processGraphNode(node.second, graph_index, last, visited);
			first_index_map.insert({graph_index, node.second->getNodeRef()});
			last_index_map.insert({graph_index, last});
			graph_index++;
		}
	}
}

void PKB::ControlFlowGraph::processGraphNode(const shared_ptr<NodeInterface>& node, size_t graph_index, StmtRef& last,
                                             unordered_set<shared_ptr<NodeInterface>>& visited) {
	node->setGraphIndex(graph_index);
	visited.insert(node);
	last = max(last, node->getNodeRef());
	if (node->getNextNodes().empty() || (node->getNodeType() == NodeType::While && node->getNextNodes().size() == 1)) {
		end_map.insert({graph_index, node});
		if (node->getNextNodes().empty()) {
			return;
		}
	}
	for (const auto& next : node->getNextNodes()) {
		if (visited.find(next) != visited.end()) {
			continue;
		}
		processGraphNode(next, graph_index, last, visited);
	}
}

void PKB::ControlFlowGraph::clear() {
	statement_node_map.clear();
	start_map.clear();
	end_map.clear();
}

unordered_set<shared_ptr<PKB::StatementNode>> PKB::ControlFlowGraph::getPreviousNodes(StmtRef index) const {
	auto node = getNode(index);
	if (node == nullptr) {
		return {};
	}
	unordered_set<shared_ptr<StatementNode>> result_set;
	for (const auto& previous : node->getPreviousNodes()) {
		if (previous->getNodeType() == NodeType::Dummy) {
			auto previous_set = collectPreviousOfDummy(previous);
			for (const auto& real : previous_set) {
				result_set.insert(getNode(real->getIdentifier()));
			}
			continue;
		}
		assert(dynamic_pointer_cast<StatementNode>(previous) != nullptr);
		result_set.insert(dynamic_pointer_cast<StatementNode>(previous));
	}
	return result_set;
}


unordered_set<shared_ptr<PKB::StatementNode>> PKB::ControlFlowGraph::getNextNodes(StmtRef index) const {
	auto node = getNode(index);
	if (node == nullptr) {
		return {};
	}
	unordered_set<shared_ptr<StatementNode>> result_set;
	for (const auto& previous : node->getNextNodes()) {
		if (previous->getNodeType() == NodeType::Dummy) {
			auto previous_set = collectNextOfDummy(previous);
			for (const auto& real : previous_set) {
				result_set.insert(getNode(real->getIdentifier()));
			}
			continue;
		}
		assert(dynamic_pointer_cast<StatementNode>(previous) != nullptr);
		result_set.insert(dynamic_pointer_cast<StatementNode>(previous));
	}
	return result_set;
}


