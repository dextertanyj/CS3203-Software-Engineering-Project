#include "NextManager.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>

using namespace std;

PKB::NextManager::NextManager(ControlFlowGraph& control_flow_graph) : control_flow_graph(control_flow_graph) {}

bool PKB::NextManager::checkNext(StmtRef first, StmtRef second) {
	if (!control_flow_graph.contains(first) || !control_flow_graph.contains(second)) {
		return false;
	}
	auto next_set = control_flow_graph.getNextNodes(first);
	return any_of(next_set.begin(), next_set.end(), [&](const StmtInfoPtr& info) { return info->getIdentifier() == second; });
}

bool PKB::NextManager::checkNextStar(StmtRef first, StmtRef second) {
	if (!control_flow_graph.contains(first) || !control_flow_graph.contains(second)) {
		return false;
	}
	if (next_cache.find(first) != next_cache.end()) {
		StmtInfoPtrSet next_set = next_cache.at(first);
		return any_of(next_set.begin(), next_set.end(), [&](const StmtInfoPtr& info) { return info->getIdentifier() == second; });
	}
	if (previous_cache.find(second) != previous_cache.end()) {
		StmtInfoPtrSet previous_set = previous_cache.at(second);
		return any_of(previous_set.begin(), previous_set.end(), [&](const StmtInfoPtr& info) { return info->getIdentifier() == first; });
	}
	if (control_flow_graph.getGraphIndex(first) != control_flow_graph.getGraphIndex(second)) {
		return false;
	}
	return checkNextStarOptimized(first, second);
}

StmtInfoPtrSet PKB::NextManager::getNext(StmtRef node_ref) {
	if (!control_flow_graph.contains(node_ref)) {
		return {};
	}
	return control_flow_graph.getNextNodes(node_ref);
}

StmtInfoPtrSet PKB::NextManager::getNextStar(StmtRef node_ref) {
	if (!control_flow_graph.contains(node_ref)) {
		return {};
	}
	if (next_cache.find(node_ref) != next_cache.end()) {
		return next_cache.find(node_ref)->second;
	}
	TraversalInformation info = {next_cache, &ControlFlowGraph::getNextNodes, &ControlFlowGraph::getLoopExternalNextNodes};
	auto node = control_flow_graph.getStatementInfo(node_ref);
	StmtInfoPQ<LessComparator> queue = constructQueue<LessComparator>(node, info);
	while (!queue.empty()) {
		StmtInfoPtr current_node = queue.top();
		queue.pop();
		processQueue(current_node, info);
	}
	return next_cache.find(node_ref)->second;
}

StmtInfoPtrSet PKB::NextManager::getPrevious(StmtRef node_ref) {
	if (!control_flow_graph.contains(node_ref)) {
		return {};
	}
	return control_flow_graph.getPreviousNodes(node_ref);
}

StmtInfoPtrSet PKB::NextManager::getPreviousStar(StmtRef node_ref) {
	if (!control_flow_graph.contains(node_ref)) {
		return {};
	}
	if (previous_cache.find(node_ref) != previous_cache.end()) {
		return previous_cache.find(node_ref)->second;
	}
	TraversalInformation info = {previous_cache, &ControlFlowGraph::getPreviousNodes, &ControlFlowGraph::getLoopExternalPreviousNodes};
	auto node = control_flow_graph.getStatementInfo(node_ref);
	StmtInfoPQ<GreaterComparator> queue = constructQueue<GreaterComparator>(node, info);
	while (!queue.empty()) {
		StmtInfoPtr current_node = queue.top();
		queue.pop();
		processQueue(current_node, info);
	}
	return previous_cache.find(node_ref)->second;
}

void PKB::NextManager::resetCache() {
	next_cache.clear();
	previous_cache.clear();
}

bool PKB::NextManager::checkNextStarOptimized(const StmtRef& first_node, const StmtRef& second_node) {
	assert(control_flow_graph.getGraphIndex(first_node) == control_flow_graph.getGraphIndex(second_node));

	// Select search direction based on the estimated distance between the start/end of the graph and the target nodes.
	auto graph_index = control_flow_graph.getGraphIndex(first_node);
	auto start = control_flow_graph.getFirstIndex(graph_index);
	auto end = control_flow_graph.getLastIndex(graph_index);
	size_t distance_to_end = end - first_node;
	size_t distance_to_start = start - second_node;
	if (distance_to_end < distance_to_start) {
		StmtInfoPtrSet next_set = getNextStar(first_node);
		return any_of(next_set.begin(), next_set.end(), [&](const StmtInfoPtr& info) { return info->getIdentifier() == second_node; });
	}
	StmtInfoPtrSet previous_set = getPreviousStar(second_node);
	return any_of(previous_set.begin(), previous_set.end(), [&](const StmtInfoPtr& info) { return info->getIdentifier() == first_node; });
}

template <class Comparator>
PKB::NextManager::StmtInfoPQ<Comparator> PKB::NextManager::constructQueue(const StmtInfoPtr& origin, TraversalInformation& info) {
	// Perform a DFS on the CFG to construct the list of nodes that require computation for the final result.
	StmtInfoPQ<Comparator> priority_queue;
	queue<StmtInfoPtr> queue;
	QueueConstructionInformation<Comparator> construction_info = {origin, priority_queue, queue, info};

	queue.push(origin);
	priority_queue.push(origin);
	while (!queue.empty()) {
		StmtInfoPtr node = queue.front();
		queue.pop();
		// If a cached entry is found, all subsequent nodes of the branch can be ignored since the cache is available for use.
		if (info.cache.find(node->getIdentifier()) != info.cache.end()) {
			continue;
		}
		constructQueueIteration(node, construction_info);
	}

	return priority_queue;
}

template <class Comparator>
void PKB::NextManager::constructQueueIteration(const StmtInfoPtr& node, QueueConstructionInformation<Comparator>& info) {
	// Loop nodes are a special case as internal nodes do not have to be computed explicitly since there is special handling during
	// processing.
	if (node->getType() == StmtType::While) {
		constructQueueLoopNode(node, info);
		return;
	}
	auto current_set = (control_flow_graph.*info.traversal_information.gatherer)(node->getIdentifier());
	for (const auto& neighbour : current_set) {
		info.priority_queue.push(neighbour);
		info.queue.push(neighbour);
	}
}

void PKB::NextManager::processQueue(const StmtInfoPtr& node, TraversalInformation& info) {
	if (node->getType() == StmtType::While) {
		processLoopNode(node, info);
		return;
	}
	StmtInfoPtrSet star;
	StmtInfoPtrSet set = (control_flow_graph.*info.gatherer)(node->getIdentifier());
	for (const auto& current : set) {
		star.emplace(current);
		auto cached_set = info.cache.find(current->getIdentifier())->second;
		star.insert(cached_set.begin(), cached_set.end());
	}
	info.cache.emplace(node->getIdentifier(), star);
}

template <class Comparator>
void PKB::NextManager::constructQueueLoopNode(const StmtInfoPtr& node, QueueConstructionInformation<Comparator>& info) {
	// If the origin is an internal node, then all nodes in the priority queue and queue are internal nodes.
	// The existing nodes in the priority queue and nodes to be searched can be cleared and only the loop node must be added into the
	// priority queue.
	bool after_entry = info.origin->getIdentifier() > node->getIdentifier();
	auto external_exit = control_flow_graph.getLoopExternalNextNodes(node->getIdentifier());
	bool before_exit = external_exit.empty() || all_of(external_exit.begin(), external_exit.end(), [&](const auto& outer) {
						   return outer->getIdentifier() > info.origin->getIdentifier();
					   });
	if (after_entry && before_exit) {
		info.priority_queue = StmtInfoPQ<Comparator>();
		info.priority_queue.push(node);
		info.queue = queue<StmtInfoPtr>();
	}
	// External nodes of the loop node are set to continue the DFS traversal.
	auto external = (control_flow_graph.*info.traversal_information.loop_continuation_handler)(node->getIdentifier());
	for (const auto& external_node : external) {
		info.priority_queue.push(external_node);
		info.queue.push(external_node);
	}
}

void PKB::NextManager::processLoopNode(const StmtInfoPtr& node, TraversalInformation& info) {
	// All internal nodes of a loop node are next and previous star of each other.
	auto loop_nodes = traverseLoop(node);
	auto combined = loop_nodes;
	StmtInfoPtrSet subsequent_nodes = ((control_flow_graph.*info.loop_continuation_handler)(node->getIdentifier()));
	if (!subsequent_nodes.empty()) {
		for (const auto& subsequent : subsequent_nodes) {
			auto subsequent_next_star = info.cache.find(subsequent->getIdentifier())->second;
			combined.merge(subsequent_next_star);
			combined.emplace(subsequent);
		}
	}
	for (const auto& internal_node : loop_nodes) {
		info.cache.emplace(internal_node->getIdentifier(), combined);
	}
}

StmtInfoPtrSet PKB::NextManager::traverseLoop(const StmtInfoPtr& node) {
	// Perform a DFS traversal on the loop node to collect all its internal nodes.
	assert(node->getType() == StmtType::While);
	StmtInfoPtrSet set;
	queue<StmtInfoPtr> queue;
	set.emplace(node);
	auto next_internal_nodes = control_flow_graph.getLoopInternalNextNodes(node->getIdentifier());
	assert(next_internal_nodes.size() == 1);
	queue.push(*next_internal_nodes.begin());
	while (!queue.empty()) {
		StmtInfoPtr current = queue.front();
		queue.pop();
		handleTraverseLoopNode(queue, set, current);
	}
	return set;
}

void PKB::NextManager::handleTraverseLoopNode(queue<StmtInfoPtr>& queue, StmtInfoPtrSet& set, const StmtInfoPtr& node) {
	set.emplace(node);
	auto next_set = control_flow_graph.getNextNodes(node->getIdentifier());
	for (const auto& next : next_set) {
		if (set.find(next) != set.end()) {
			continue;
		}
		queue.push(next);
	}
}
