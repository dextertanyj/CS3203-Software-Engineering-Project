#include "NextManager.h"

#include <algorithm>
#include <cassert>
#include <queue>

struct LessComparator {
	bool operator()(const shared_ptr<PKB::StatementNode> &lhs, const shared_ptr<PKB::StatementNode> &rhs) const { return *lhs < *rhs; }
};

struct GreaterComparator {
	bool operator()(const shared_ptr<PKB::StatementNode> &lhs, const shared_ptr<PKB::StatementNode> &rhs) const { return *lhs > *rhs; }
};

PKB::NextManager::NextManager(ControlFlowGraph &control_flow_graph) : control_flow_graph(&control_flow_graph) {}

void PKB::NextManager::setNext(StmtRef previous, StmtRef next) {
	if (previous == next) {
		throw invalid_argument("Cannot set a node's direct next to itself.");
	}
	try {
		auto prev_node = control_flow_graph->getNode(previous);
		auto next_node = control_flow_graph->getNode(next);
		prev_node->setConnection(next_node);
	} catch (const invalid_argument &e) {
		throw e;
	}
}

void PKB::NextManager::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	if (prev >= then_next || prev >= else_next || then_next >= else_next) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	try {
		auto prev_node = control_flow_graph->getNode(prev);
		auto then_next_node = control_flow_graph->getNode(then_next);
		auto else_next_node = control_flow_graph->getNode(else_next);
		if (prev_node->getNodeType() != NodeType::If) {
			throw invalid_argument("First argument must refer to an if statement.");
		}
		shared_ptr<PKB::IfNode> if_ctrl_node = dynamic_pointer_cast<PKB::IfNode>(prev_node);
		if_ctrl_node->insertIfNext(then_next_node, else_next_node);
	} catch (const invalid_argument &e) {
		throw e;
	}
}

void PKB::NextManager::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	if (if_stmt_ref >= then_prev || if_stmt_ref >= else_prev || then_prev >= else_prev) {
		throw invalid_argument("Ordering or value(s) of provided statement references is invalid.");
	}
	try {
		auto then_prev_node = control_flow_graph->getNode(then_prev);
		auto else_prev_node = control_flow_graph->getNode(else_prev);
		auto if_ctrl_node = control_flow_graph->getNode(if_stmt_ref);
		if (if_ctrl_node->getNodeType() != NodeType::If) {
			throw invalid_argument("Third argument must refer to an if control statement.");
		}
		shared_ptr<PKB::IfNode> if_node = dynamic_pointer_cast<PKB::IfNode>(if_ctrl_node);
		if_node->insertIfExit(then_prev_node, else_prev_node);
	} catch (const invalid_argument &e) {
		throw e;
	}
}

bool PKB::NextManager::checkNext(StmtRef first, StmtRef second) {
	try {
		auto prev_node = control_flow_graph->getNode(first);
		auto next_node = control_flow_graph->getNode(second);
		StmtInfoPtrSet next_nodes_of_prev = this->getNext(first);
		return any_of(next_nodes_of_prev.begin(), next_nodes_of_prev.end(),
		              [second](const shared_ptr<StmtInfo> &next_info) { return next_info->getIdentifier() == second; });
	} catch (const invalid_argument &e) {
		throw e;
	}
}

bool PKB::NextManager::checkNextStar(StmtRef first, StmtRef second) {
	if (next_cache.find(first) != next_cache.end()) {
		StmtInfoPtrSet next_set = next_cache.at(first);
		return any_of(next_set.begin(), next_set.end(), [&](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == second; });
	}
	if (previous_cache.find(second) != previous_cache.end()) {
		StmtInfoPtrSet previous_set = previous_cache.at(second);
		return any_of(previous_set.begin(), previous_set.end(),
		              [&](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == first; });
	}
	auto first_node = control_flow_graph->getNode(first);
	auto second_node = control_flow_graph->getNode(second);
	if (first_node->getGraphIndex() != second_node->getGraphIndex()) {
		return false;
	}
	return checkNextStarOptimized(first_node, second_node);
}

StmtInfoPtrSet PKB::NextManager::getNext(StmtRef node_ref) {
	shared_ptr<PKB::StatementNode> curr_node = control_flow_graph->getNode(node_ref);
	StmtInfoPtrSet next_nodes;
	for (const auto &node : curr_node->getNextNodes()) {
		if (node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet next_of_dummy = control_flow_graph->collectNextOfDummy(node);
			next_nodes.insert(next_of_dummy.begin(), next_of_dummy.end());
		} else {
			shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(node);
			next_nodes.insert(stmt_node->getStmtInfo());
		}
	}
	return next_nodes;
}

StmtInfoPtrSet PKB::NextManager::getNextStar(StmtRef node_ref) {
	if (next_cache.find(node_ref) != next_cache.end()) {
		return next_cache.find(node_ref)->second;
	}
	shared_ptr<StatementNode> target = control_flow_graph->getNode(node_ref);
	TraversalInformation info = {next_cache, &NodeInterface::getNextNodes, &NextManager::processLoopExit,
	                             &ControlFlowGraph::collectNextOfDummy};
	priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, LessComparator> queue =
		constructQueue<LessComparator>(target, info);
	while (!queue.empty()) {
		shared_ptr<StatementNode> current_node = queue.top();
		queue.pop();
		processQueue(current_node, info);
	}
	return next_cache.find(node_ref)->second;
}

StmtInfoPtrSet PKB::NextManager::getPrevious(StmtRef node_ref) {
	shared_ptr<PKB::StatementNode> curr_node = this->control_flow_graph->getNode(node_ref);
	StmtInfoPtrSet prev_nodes;
	for (const auto &node : curr_node->getPreviousNodes()) {
		// If previous node is a dummy node, need to get the previous nodes of the dummy node.
		if (node->getNodeType() == NodeType::Dummy) {
			StmtInfoPtrSet prev_of_dummy = control_flow_graph->collectPreviousOfDummy(node);
			prev_nodes.insert(prev_of_dummy.begin(), prev_of_dummy.end());
		} else {
			shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<StatementNode>(node);
			prev_nodes.insert(stmt_node->getStmtInfo());
		}
	}
	return prev_nodes;
}

StmtInfoPtrSet PKB::NextManager::getPreviousStar(StmtRef node_ref) {
	if (previous_cache.find(node_ref) != previous_cache.end()) {
		return previous_cache.find(node_ref)->second;
	}
	shared_ptr<StatementNode> target = control_flow_graph->getNode(node_ref);
	TraversalInformation info = {previous_cache, &NodeInterface::getPreviousNodes, &NextManager::processLoopEntry,
	                             &ControlFlowGraph::collectPreviousOfDummy};
	priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, GreaterComparator> queue =
		constructQueue<GreaterComparator>(target, info);
	while (!queue.empty()) {
		shared_ptr<StatementNode> current_node = queue.top();
		queue.pop();
		processQueue(current_node, info);
	}
	return previous_cache.find(node_ref)->second;
}

void PKB::NextManager::resetCache() {
	next_cache.clear();
	previous_cache.clear();
}

bool PKB::NextManager::checkNextStarOptimized(const shared_ptr<StatementNode> &first_node, const shared_ptr<StatementNode> &second_node) {
	StmtRef first = first_node->getNodeRef();
	StmtRef second = second_node->getNodeRef();
	auto start = control_flow_graph->getStart(first_node->getGraphIndex());
	auto end = control_flow_graph->getEnd(first_node->getGraphIndex());
	shared_ptr<StatementNode> true_start = dynamic_pointer_cast<StatementNode>(start);
	shared_ptr<StatementNode> true_end;
	if (end->getNodeType() == NodeType::Dummy) {
		set<shared_ptr<StatementNode>, LessComparator> set;
		handleDummyNodeSearch(set, end, &ControlFlowGraph::collectPreviousOfDummy);
		assert(!set.empty());
		true_end = *set.rbegin();
	} else {
		true_end = dynamic_pointer_cast<StatementNode>(start);
	}
	size_t distance_to_end = true_end->getNodeRef() - first_node->getNodeRef();
	size_t distance_to_start = second_node->getNodeRef() - true_start->getNodeRef();
	if (distance_to_end < distance_to_start) {
		StmtInfoPtrSet next_set = getNextStar(first);
		return any_of(next_set.begin(), next_set.end(), [&](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == second; });
	}
	StmtInfoPtrSet previous_set = getPreviousStar(second);
	return any_of(previous_set.begin(), previous_set.end(),
	              [&](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == first; });
}

template <class Comparator>
priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, Comparator> PKB::NextManager::constructQueue(
	const shared_ptr<StatementNode> &origin, TraversalInformation &info) {
	// Perform a DFS on the CFG to construct the list of nodes that require computation for the final result.
	priority_queue<shared_ptr<StatementNode>, vector<shared_ptr<StatementNode>>, Comparator> priority_queue;
	queue<shared_ptr<StatementNode>> queue;
	QueueConstructionInformation<Comparator> construction_info = {origin, priority_queue, queue, info};
	queue.push(origin);
	priority_queue.push(origin);
	while (!queue.empty()) {
		shared_ptr<StatementNode> node = queue.front();
		queue.pop();
		// If a cached entry is found, all subsequent nodes of the branch can be ignored since the cache is available for use.
		if (info.cache.find(node->getNodeRef()) != info.cache.end()) {
			continue;
		}
		constructQueueIteration(node, construction_info);
	}
	return priority_queue;
}

template <class Comparator>
void PKB::NextManager::constructQueueIteration(const shared_ptr<StatementNode> &node, QueueConstructionInformation<Comparator> &info) {
	// Loop nodes are a special case as internal nodes do not have to be computed explicitly since there is special handling during
	// processing.
	if (node->getNodeType() == NodeType::While) {
		constructQueueLoopNode(node, info);
		return;
	}
	auto current_set = (*node.*info.traversal_information.gatherer)();
	for (const auto &neighbour : current_set) {
		if (neighbour->getNodeType() == NodeType::Dummy) {
			handleDummyNodeSearch(info.queue, neighbour, info.traversal_information.collector);
			handleDummyNodeSearch(info.priority_queue, neighbour, info.traversal_information.collector);
			continue;
		}
		shared_ptr<StatementNode> neighbour_statement = dynamic_pointer_cast<StatementNode>(neighbour);
		info.priority_queue.push(neighbour_statement);
		info.queue.push(neighbour_statement);
	}
}

void PKB::NextManager::processQueue(const shared_ptr<StatementNode> &node, TraversalInformation &info) {
	if (node->getNodeType() == NodeType::While) {
		processLoopNode(node, info);
		return;
	}
	StmtInfoPtrSet star;
	unordered_set<shared_ptr<NodeInterface>> set = (*node.*info.gatherer)();
	while (!set.empty()) {
		shared_ptr<NodeInterface> current = *(set.begin());
		set.erase(current);
		if (current->getNodeType() == NodeType::Dummy) {
			handleDummyNodeSearch(set, current, info.collector);
			continue;
		}
		shared_ptr<StatementNode> statement = dynamic_pointer_cast<StatementNode>(current);
		star.insert(statement->getStmtInfo());
		auto cached_set = info.cache.find(statement->getNodeRef())->second;
		star.insert(cached_set.begin(), cached_set.end());
	}
	info.cache.insert({node->getNodeRef(), star});
}

template <class Comparator>
void PKB::NextManager::constructQueueLoopNode(const shared_ptr<StatementNode> &node, QueueConstructionInformation<Comparator> &info) {
	auto outer_nodes = processLoopExit(node).second;
	// If the origin is an internal node, then all nodes in the priority queue and queue are internal nodes.
	// The existing nodes in the priority queue and nodes to be searched can be cleared and only the loop node must be added into the
	// priority queue.
	if (info.origin->getNodeRef() > node->getNodeRef() &&
	    (outer_nodes.empty() || all_of(outer_nodes.begin(), outer_nodes.end(),
	                                   [&](const auto &outer) { return outer->getNodeRef() > info.origin->getNodeRef(); }))) {
		info.priority_queue = std::priority_queue<shared_ptr<StatementNode>, vector<shared_ptr<StatementNode>>, Comparator>();
		info.priority_queue.push(node);
		info.queue = std::queue<shared_ptr<StatementNode>>();
	}
	// External nodes of the loop node are set to continue the DFS traversal.
	auto external = (this->*info.traversal_information.loop_continuation_handler)(node).second;
	for (const auto &external_node : external) {
		info.priority_queue.push(external_node);
		info.queue.push(external_node);
	}
}

void PKB::NextManager::processLoopNode(const shared_ptr<StatementNode> &node, TraversalInformation &info) {
	// All internal nodes of a loop node are next and previous star of each other.
	auto loop_nodes = traverseLoop(node);
	auto combined = loop_nodes;
	unordered_set<shared_ptr<StatementNode>> subsequent_nodes = ((this->*info.loop_continuation_handler)(node)).second;
	if (!subsequent_nodes.empty()) {
		for (const auto &subsequent : subsequent_nodes) {
			auto subsequent_next_star = info.cache.find(subsequent->getNodeRef())->second;
			combined.merge(subsequent_next_star);
			combined.insert(subsequent->getStmtInfo());
		}
	}
	for (const auto &internal_node : loop_nodes) {
		info.cache.insert({internal_node->getIdentifier(), combined});
	}
}

StmtInfoPtrSet PKB::NextManager::traverseLoop(const shared_ptr<NodeInterface> &node) {
	// Perform a DFS traversal on the loop node to collect all its internal nodes.
	assert(node->getNodeType() == NodeType::While);
	shared_ptr<StatementNode> loop_node = dynamic_pointer_cast<StatementNode>(node);
	StmtInfoPtrSet set;
	queue<shared_ptr<NodeInterface>> queue;
	set.insert(loop_node->getStmtInfo());
	queue.push(*processLoopExit(loop_node).first.begin());
	while (!queue.empty()) {
		shared_ptr<NodeInterface> current = queue.front();
		queue.pop();
		handleTraverseLoopNode(queue, set, current);
	}
	return set;
}

void PKB::NextManager::handleTraverseLoopNode(queue<shared_ptr<NodeInterface>> &queue, StmtInfoPtrSet &set,
                                              const shared_ptr<NodeInterface> &node) {
	if (node->getNodeType() == NodeType::Dummy) {
		handleDummyNodeSearch(queue, node, &ControlFlowGraph::collectNextOfDummy);
		return;
	}
	shared_ptr<StatementNode> current_statement = dynamic_pointer_cast<StatementNode>(node);
	set.insert(current_statement->getStmtInfo());
	auto next_set = current_statement->getNextNodes();
	for (const auto &next : next_set) {
		if (next->getNodeType() == NodeType::Dummy) {
			queue.push(next);
			continue;
		}
		auto statement_node = dynamic_pointer_cast<StatementNode>(next);
		if (set.find(statement_node->getStmtInfo()) != set.end()) {
			continue;
		}
		queue.push(next);
	}
}

template <typename Comparator>
PKB::NextManager::LoopNodePair PKB::NextManager::processLoopEntryExit(
	const shared_ptr<PKB::StatementNode> &loop_node, unordered_set<shared_ptr<PKB::NodeInterface>> (NodeInterface::*gatherer)() const,
	StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	assert(loop_node->getNodeType() == PKB::NodeType::While);
	unordered_set<shared_ptr<PKB::NodeInterface>> node_set = (*loop_node.*gatherer)();
	assert(!node_set.empty());
	// Internal nodes always exist.
	if (node_set.size() == 1) {
		unordered_set<shared_ptr<StatementNode>> result;
		shared_ptr<NodeInterface> node = *node_set.begin();
		if (node->getNodeType() == NodeType::Dummy) {
			handleDummyNodeSearch(result, node, collector);
			return {result, {}};
		}
		assert(dynamic_pointer_cast<StatementNode>(node) != nullptr);
		result.insert(dynamic_pointer_cast<StatementNode>(node));
		return {result, {}};
	}
	auto first_node = *(node_set.begin());
	unordered_set<shared_ptr<PKB::StatementNode>> first_set = checkLoopNeighbour(first_node, collector);
	auto second_node = *(++node_set.begin());
	unordered_set<shared_ptr<PKB::StatementNode>> second_set = checkLoopNeighbour(second_node, collector);
	// Sets can possibly be empty as the exit node may be a terminal dummy node.
	if (first_set.empty()) {
		return {second_set, first_set};
	}
	if (second_set.empty()) {
		return {first_set, second_set};
	}

	// Special handling for immediately nested loops.
	// Internal next node may have a statement index greater than the external next node, but will always be +1 of the loop node.
	if (any_of(first_set.begin(), first_set.end(),
	           [&](const auto &first) { return first->getNodeRef() == (loop_node->getNodeRef() + 1); })) {
		return {first_set, second_set};
	}
	if (any_of(second_set.begin(), second_set.end(),
	           [&](const auto &second) { return second->getNodeRef() == (loop_node->getNodeRef() + 1); })) {
		return {second_set, first_set};
	}

	if (Comparator{}(second_set.begin()->get()->getNodeRef(), first_set.begin()->get()->getNodeRef())) {
		return {second_set, first_set};
	}
	return {first_set, second_set};
}

PKB::NextManager::LoopNodePair PKB::NextManager::processLoopEntry(const shared_ptr<PKB::StatementNode> &node) {
	return processLoopEntryExit<std::greater<StmtRef>>(node, &NodeInterface::getPreviousNodes, &ControlFlowGraph::collectPreviousOfDummy);
}

PKB::NextManager::LoopNodePair PKB::NextManager::processLoopExit(const shared_ptr<PKB::StatementNode> &node) {
	auto result = processLoopEntryExit<std::less<StmtRef>>(node, &NodeInterface::getNextNodes, &ControlFlowGraph::collectNextOfDummy);
	assert(result.first.size() == 1);
	assert(result.second.size() < 2);
	return result;
}

unordered_set<shared_ptr<PKB::StatementNode>> PKB::NextManager::checkLoopNeighbour(
	const shared_ptr<NodeInterface> &node, StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	unordered_set<shared_ptr<StatementNode>> set;
	if (node->getNodeType() == PKB::NodeType::Dummy) {
		handleDummyNodeSearch(set, node, collector);
		return set;
	}
	assert(dynamic_pointer_cast<PKB::StatementNode>(node) != nullptr);
	set.insert(dynamic_pointer_cast<StatementNode>(node));
	return set;
}

template <class T>
void PKB::NextManager::handleDummyNodeSearch(T &queue, const shared_ptr<NodeInterface> &dummy_node,
                                             StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	auto nodes = (control_flow_graph->*collector)(dummy_node);
	for (const auto &info : nodes) {
		shared_ptr<StatementNode> node = control_flow_graph->getNode(info->getIdentifier());
		queue.emplace(node);
	}
}
