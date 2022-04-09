#include "PKB/CFG/ControlFlowGraph.h"

#include <algorithm>
#include <cassert>

#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/WhileNode.h"
#include "PKB/Types.h"

using namespace std;

void PKB::ControlFlowGraph::createNode(const StmtInfoPtr& stmt_info) {
	assert(statement_node_map.find(stmt_info->getIdentifier()) == statement_node_map.end());
	// Check node type and create respective node.
	if (stmt_info->getType() == StmtType::If) {
		shared_ptr<IfNode> to_insert = make_shared<IfNode>(stmt_info);
		statement_node_map.emplace(stmt_info->getIdentifier(), to_insert);

	} else if (stmt_info->getType() == StmtType::While) {
		shared_ptr<WhileNode> to_insert = make_shared<WhileNode>(stmt_info);
		statement_node_map.emplace(stmt_info->getIdentifier(), to_insert);
	} else {
		shared_ptr<StatementNode> to_insert = make_shared<StatementNode>(stmt_info);
		statement_node_map.emplace(stmt_info->getIdentifier(), to_insert);
	}
}

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

	shared_ptr<IfNode> if_ctrl_node = dynamic_pointer_cast<IfNode>(prev_node);
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

	shared_ptr<IfNode> if_node = dynamic_pointer_cast<IfNode>(if_ctrl_node);
	if_node->insertIfExit(then_prev_node, else_prev_node);
}

bool PKB::ControlFlowGraph::contains(StmtRef index) const { return statement_node_map.find(index) != statement_node_map.end(); }

StmtType PKB::ControlFlowGraph::getType(StmtRef ref) const {
	auto node = getNode(ref);
	assert(node != nullptr);
	return node->getStmtInfo()->getType();
}

StmtInfoPtr PKB::ControlFlowGraph::getStmtInfo(StmtRef index) const {
	auto node = getNode(index);
	assert(node != nullptr);
	return node->getStmtInfo();
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

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopExternalNextNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair = groupLoopNeighbouringNodes<LessComparator>(index, &NodeInterface::getNextNodes, &ControlFlowGraph::collectNextOfDummy);
	return pair.second;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopExternalPreviousNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair =
		groupLoopNeighbouringNodes<GreaterComparator>(index, &NodeInterface::getPreviousNodes, &ControlFlowGraph::collectPreviousOfDummy);
	return pair.second;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopInternalNextNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair = groupLoopNeighbouringNodes<LessComparator>(index, &NodeInterface::getNextNodes, &ControlFlowGraph::collectNextOfDummy);
	return pair.first;
}

StmtInfoPtrSet PKB::ControlFlowGraph::getLoopInternalPreviousNodes(StmtRef index) const {
	assert(getNode(index)->getNodeType() == Types::NodeType::While);
	auto pair =
		groupLoopNeighbouringNodes<GreaterComparator>(index, &NodeInterface::getPreviousNodes, &ControlFlowGraph::collectPreviousOfDummy);
	return pair.first;
}

size_t PKB::ControlFlowGraph::getGraphIndex(StmtRef index) const {
	auto node = getNode(index);
	assert(node != nullptr);
	return node->getGraphIndex();
}

StmtRef PKB::ControlFlowGraph::getFirstIndex(size_t graph_index) const {
	assert(first_index_map.find(graph_index) != first_index_map.end());
	return first_index_map.at(graph_index);
}

StmtRef PKB::ControlFlowGraph::getLastIndex(size_t graph_index) const {
	assert(last_index_map.find(graph_index) != last_index_map.end());
	return last_index_map.at(graph_index);
}

void PKB::ControlFlowGraph::optimize() {
	size_t graph_index = 0;
	for (const auto& node : statement_node_map) {
		// Starting while nodes have only a single previous node since internal nodes always exist.
		if (node.second->getPreviousNodes().empty() ||
		    (node.second->getNodeType() == Types::NodeType::While && node.second->getPreviousNodes().size() == 1)) {
			StmtRef last = node.second->getNodeRef();
			unordered_set<shared_ptr<NodeInterface>> visited;
			processGraphNode(node.second, graph_index, last, visited);
			first_index_map.emplace(graph_index, node.second->getNodeRef());
			last_index_map.emplace(graph_index, last);
			graph_index++;
		}
	}
}

void PKB::ControlFlowGraph::clear() {
	statement_node_map.clear();
	first_index_map.clear();
	last_index_map.clear();
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

	shared_ptr<StatementNode> stmt_node = dynamic_pointer_cast<StatementNode>(result);
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
			shared_ptr<StatementNode> stmt_node = dynamic_pointer_cast<StatementNode>(prev_node);
			assert(stmt_node != nullptr);
			collection.emplace(stmt_node->getStmtInfo());
		}
	}
	return collection;
}

shared_ptr<PKB::StatementNode> PKB::ControlFlowGraph::getNode(StmtRef index) const {
	auto iter = statement_node_map.find(index);
	assert(iter != statement_node_map.end());
	return statement_node_map.find(index)->second;
}

void PKB::ControlFlowGraph::processGraphNode(const shared_ptr<NodeInterface>& node, size_t graph_index, StmtRef& last,
                                             unordered_set<shared_ptr<NodeInterface>>& visited) {
	node->setGraphIndex(graph_index);
	visited.emplace(node);
	last = max(last, node->getNodeRef());
	for (const auto& next : node->getNextNodes()) {
		if (visited.find(next) != visited.end()) {
			continue;
		}
		processGraphNode(next, graph_index, last, visited);
	}
}

template <typename Comparator>
PKB::ControlFlowGraph::LoopNodePair PKB::ControlFlowGraph::groupLoopNeighbouringNodes(StmtRef index, NodeGatherer gatherer,
                                                                                      Collector collector) const {
	auto node = getNode(index);

	assert(getNode(index)->getNodeType() == Types::NodeType::While);

	unordered_set<shared_ptr<NodeInterface>> node_set = (*node.*gatherer)();
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
	assert(dynamic_pointer_cast<StatementNode>(node) != nullptr);
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
