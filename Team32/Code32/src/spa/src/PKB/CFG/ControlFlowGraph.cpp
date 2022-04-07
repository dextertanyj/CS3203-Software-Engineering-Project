#include "PKB/CFG/ControlFlowGraph.h"

#include <cassert>

#include "PKB/CFG/DummyNode.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/CFG/WhileNode.h"
#include "PKB/Types.h"

void PKB::ControlFlowGraph::setNext(StmtRef previous, StmtRef next) const {
	assert(previous != next);
	auto prev_node = getNode(previous);
	auto next_node = getNode(next);
	assert(prev_node != nullptr && next_node != nullptr);
	prev_node->setConnection(next_node);
}

void PKB::ControlFlowGraph::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) const {
	assert(prev < then_next);
	assert(prev < else_next);
	assert(then_next < else_next);

	auto prev_node = getNode(prev);
	assert(prev_node->getNodeType() == Types::NodeType::If);

	auto then_next_node = getNode(then_next);
	auto else_next_node = getNode(else_next);

	shared_ptr<PKB::IfNode> if_ctrl_node = dynamic_pointer_cast<PKB::IfNode>(prev_node);
	if_ctrl_node->insertIfNext(then_next_node, else_next_node);
}

void PKB::ControlFlowGraph::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) const {
	assert(if_stmt_ref < then_prev);
	assert(if_stmt_ref < then_prev);
	assert(then_prev < else_prev);

	auto if_ctrl_node = getNode(if_stmt_ref);
	assert(if_ctrl_node->getNodeType() == Types::NodeType::If);

	auto then_prev_node = getNode(then_prev);
	auto else_prev_node = getNode(else_prev);

	shared_ptr<PKB::IfNode> if_node = dynamic_pointer_cast<PKB::IfNode>(if_ctrl_node);
	if_node->insertIfExit(then_prev_node, else_prev_node);
}

void PKB::ControlFlowGraph::createNode(const shared_ptr<StmtInfo>& stmt_info) {
	assert(statement_node_map.find(stmt_info->getIdentifier()) == statement_node_map.end());
	// Check node type and create respective node.
	if (stmt_info->getType() == StmtType::If) {
		shared_ptr<PKB::IfNode> to_insert = make_shared<PKB::IfNode>(stmt_info);
		statement_node_map.emplace(stmt_info->getIdentifier(), to_insert);

	} else if (stmt_info->getType() == StmtType::While) {
		shared_ptr<PKB::WhileNode> to_insert = make_shared<PKB::WhileNode>(stmt_info);
		statement_node_map.emplace(stmt_info->getIdentifier(), to_insert);
	} else {
		shared_ptr<PKB::StatementNode> to_insert = make_shared<PKB::StatementNode>(stmt_info);
		statement_node_map.emplace(stmt_info->getIdentifier(), to_insert);
	}
}

bool PKB::ControlFlowGraph::contains(StmtRef index) const { return statement_node_map.find(index) != statement_node_map.end(); }

shared_ptr<PKB::StatementNode> PKB::ControlFlowGraph::getNode(StmtRef ref) const {
	auto iter = statement_node_map.find(ref);
	assert(iter != statement_node_map.end());
	return statement_node_map.find(ref)->second;
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
	auto node = getNode(ref);
	assert(node != nullptr);
	return node->getGraphIndex();
}

StmtRef PKB::ControlFlowGraph::getFirstIndex(size_t graph_index) const {
	assert(start_map.find(graph_index) != start_map.end());
	return first_index_map.at(graph_index);
}

StmtRef PKB::ControlFlowGraph::getLastIndex(size_t graph_index) const {
	assert(start_map.find(graph_index) != start_map.end());
	return last_index_map.at(graph_index);
}

shared_ptr<StmtInfo> PKB::ControlFlowGraph::getStart(size_t graph_index) const {
	assert(start_map.find(graph_index) != start_map.end());
	auto node = start_map.find(graph_index)->second;
	return node->getStmtInfo();
}

StmtInfoPtrSet PKB::ControlFlowGraph::getEnd(size_t graph_index) const {
	assert(end_map.find(graph_index) != end_map.end());
	auto node = end_map.find(graph_index)->second;
	StmtInfoPtrSet results;
	if (node->getNodeType() == Types::NodeType::Dummy) {
		return collectPreviousOfDummy(node);
	}
	assert(dynamic_pointer_cast<StatementNode>(node) != nullptr);
	results.emplace(dynamic_pointer_cast<StatementNode>(node)->getStmtInfo());
	return results;
}

StmtInfoPtrSet PKB::ControlFlowGraph::collectNextOfDummy(const shared_ptr<PKB::NodeInterface>& dummy_node) {
	shared_ptr<PKB::NodeInterface> result = dummy_node;
	while (result->getNodeType() == Types::NodeType::Dummy && !(result->getNextNodes().empty())) {
		assert(result->getNextNodes().size() == 1);
		result = *result->getNextNodes().begin();
	}

	if (result->getNodeType() == Types::NodeType::Dummy) {
		return {};
	}

	shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(result);
	assert(stmt_node != nullptr);

	return {stmt_node->getStmtInfo()};
}

StmtInfoPtrSet PKB::ControlFlowGraph::collectPreviousOfDummy(const shared_ptr<PKB::NodeInterface>& dummy_node) {
	StmtInfoPtrSet collection;
	for (const auto& prev_node : dummy_node->getPreviousNodes()) {
		if (prev_node->getNodeType() == Types::NodeType::Dummy) {
			StmtInfoPtrSet sub_collection = collectPreviousOfDummy(prev_node);
			collection.insert(sub_collection.begin(), sub_collection.end());
		} else {
			shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(prev_node);
			assert(stmt_node != nullptr);
			collection.emplace(stmt_node->getStmtInfo());
		}
	}
	return collection;
}

void PKB::ControlFlowGraph::optimize() {
	size_t graph_index = 0;
	for (const auto& node : statement_node_map) {
		// Starting while nodes have only a single previous node since internal nodes always exist.
		if (node.second->getPreviousNodes().empty() ||
		    (node.second->getNodeType() == Types::NodeType::While && node.second->getPreviousNodes().size() == 1)) {
			StmtRef last = node.second->getNodeRef();
			start_map.emplace(graph_index, node.second);
			unordered_set<shared_ptr<NodeInterface>> visited;
			processGraphNode(node.second, graph_index, last, visited);
			first_index_map.emplace(graph_index, node.second->getNodeRef());
			last_index_map.emplace(graph_index, last);
			graph_index++;
		}
	}
}

void PKB::ControlFlowGraph::processGraphNode(const shared_ptr<NodeInterface>& node, size_t graph_index, StmtRef& last,
                                             unordered_set<shared_ptr<NodeInterface>>& visited) {
	node->setGraphIndex(graph_index);
	visited.emplace(node);
	last = max(last, node->getNodeRef());
	// While nodes that are exit nodes have only a single next node since internal nodes always exist.
	if (node->getNextNodes().empty() || (node->getNodeType() == Types::NodeType::While && node->getNextNodes().size() == 1)) {
		end_map.emplace(graph_index, node);
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

StmtInfoPtrSet PKB::ControlFlowGraph::getPreviousNodes(StmtRef index) const {
	auto node = getNode(index);
	if (node == nullptr) {
		return {};
	}
	StmtInfoPtrSet result_set;
	for (const auto& previous : node->getPreviousNodes()) {
		if (previous->getNodeType() == Types::NodeType::Dummy) {
			handleDummyNodeSearch(result_set, previous, &ControlFlowGraph::collectPreviousOfDummy);
			continue;
		}
		assert(dynamic_pointer_cast<StatementNode>(previous) != nullptr);
		result_set.emplace(dynamic_pointer_cast<StatementNode>(previous)->getStmtInfo());
	}
	return result_set;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getNextNodes(StmtRef index) const {
	auto node = getNode(index);
	if (node == nullptr) {
		return {};
	}
	StmtInfoPtrSet result_set;
	for (const auto& next : node->getNextNodes()) {
		if (next->getNodeType() == Types::NodeType::Dummy) {
			handleDummyNodeSearch(result_set, next, &ControlFlowGraph::collectNextOfDummy);
			continue;
		}
		assert(dynamic_pointer_cast<StatementNode>(next) != nullptr);
		result_set.emplace(dynamic_pointer_cast<StatementNode>(next)->getStmtInfo());
	}
	return result_set;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopExternalNextNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair = groupLoopNeighbouringNodes<LessComparator>(index, &NodeInterface::getNextNodes, &ControlFlowGraph::collectNextOfDummy);
	return pair.second;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopInternalNextNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair = groupLoopNeighbouringNodes<LessComparator>(index, &NodeInterface::getNextNodes, &ControlFlowGraph::collectNextOfDummy);
	return pair.first;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopExternalPreviousNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair =
		groupLoopNeighbouringNodes<GreaterComparator>(index, &NodeInterface::getPreviousNodes, &ControlFlowGraph::collectPreviousOfDummy);
	return pair.second;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopInternalPreviousNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair =
		groupLoopNeighbouringNodes<GreaterComparator>(index, &NodeInterface::getPreviousNodes, &ControlFlowGraph::collectPreviousOfDummy);
	return pair.first;
}

template <typename Comparator>
PKB::ControlFlowGraph::LoopNodePair PKB::ControlFlowGraph::groupLoopNeighbouringNodes(StmtRef index, NodeGatherer gatherer,
                                                                                      Collector collector) const {
	auto node = getNode(index);

	assert(getNode(index)->getNodeType() == Types::NodeType::While);

	unordered_set<shared_ptr<PKB::NodeInterface>> node_set = (*node.*gatherer)();
	// Internal nodes always exist.
	assert(!node_set.empty());
	if (node_set.size() == 1) {  // No external nodes.
		StmtInfoPtrSet result;
		shared_ptr<NodeInterface> internal_node = *node_set.begin();
		if (internal_node->getNodeType() == Types::NodeType::Dummy) {
			handleDummyNodeSearch(result, node, collector);
			return {result, {}};
		}
		assert(dynamic_pointer_cast<StatementNode>(node) != nullptr);
		result.emplace(dynamic_pointer_cast<StatementNode>(node)->getStmtInfo());
		return {result, {}};
	}

	auto first_node = *(node_set.begin());
	StmtInfoPtrSet first_set = collectLoopNeighbours(first_node, collector);
	auto second_node = *(++node_set.begin());
	StmtInfoPtrSet second_set = collectLoopNeighbours(second_node, collector);

	// Sets can possibly be empty as the exit node may be a terminal dummy node.
	if (first_set.empty()) {
		return {second_set, first_set};
	}
	if (second_set.empty()) {
		return {first_set, second_set};
	}

	// Special handling for immediately nested loops.
	// Internal next node may have a statement index greater than the external next node, but will always be +1 of the loop node.
	if (any_of(first_set.begin(), first_set.end(), [&](const auto& first) { return first->getIdentifier() == (index + 1); })) {
		return {first_set, second_set};
	}
	if (any_of(second_set.begin(), second_set.end(), [&](const auto& second) { return second->getIdentifier() == (index + 1); })) {
		return {second_set, first_set};
	}

	if (Comparator{}(*second_set.begin(), *first_set.begin())) {
		return {second_set, first_set};
	}

	return {first_set, second_set};
}

StmtInfoPtrSet PKB::ControlFlowGraph::collectLoopNeighbours(const shared_ptr<NodeInterface>& node, Collector collector) const {
	StmtInfoPtrSet set;
	if (node->getNodeType() == Types::NodeType::Dummy) {
		handleDummyNodeSearch(set, node, collector);
		return set;
	}
	assert(dynamic_pointer_cast<PKB::StatementNode>(node) != nullptr);
	set.emplace(dynamic_pointer_cast<StatementNode>(node)->getStmtInfo());
	return set;
}

template <class T>
void PKB::ControlFlowGraph::handleDummyNodeSearch(T& queue, const shared_ptr<NodeInterface>& dummy_node, Collector collector) const {
	auto nodes = (*collector)(dummy_node);
	for (const auto& info : nodes) {
		queue.emplace(info);
	}
}