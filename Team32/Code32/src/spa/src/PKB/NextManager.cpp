#include "NextManager.h"

#include <algorithm>
#include <cassert>
#include <queue>

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
	StmtInfoPtrSet nexts = getNextStar(first);
	return any_of(nexts.begin(), nexts.end(), [&second](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == second; });
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

struct NextComparator {
	bool operator()(const shared_ptr<PKB::StatementNode> &lhs, const shared_ptr<PKB::StatementNode> &rhs) { return *lhs < *rhs; }
};

struct PreviousComparator {
	bool operator()(const shared_ptr<PKB::StatementNode> &lhs, const shared_ptr<PKB::StatementNode> &rhs) { return *lhs > *rhs; }
};

StmtInfoPtrSet PKB::NextManager::getNextStar(StmtRef node_ref) {
	if (next_cache.find(node_ref) != next_cache.end()) {
		return next_cache.find(node_ref)->second;
	}
	shared_ptr<StatementNode> target = control_flow_graph->getNode(node_ref);
	TraversalInformation info = {&next_cache, &NodeInterface::getNextNodes, &NextManager::processLoopExit,
	                             &ControlFlowGraph::collectNextOfDummy};
	priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, NextComparator> queue =
		constructQueue<NextComparator>(target, info);
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
	TraversalInformation info = {&previous_cache, &NodeInterface::getPreviousNodes, &NextManager::processLoopEntry,
	                             &ControlFlowGraph::collectPreviousOfDummy};
	priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, PreviousComparator> queue =
		constructQueue<PreviousComparator>(target, info);
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

template <class Comparator>
priority_queue<shared_ptr<PKB::StatementNode>, vector<shared_ptr<PKB::StatementNode>>, Comparator> PKB::NextManager::constructQueue(
	const shared_ptr<StatementNode> &node, TraversalInformation &info) {
	priority_queue<shared_ptr<StatementNode>, vector<shared_ptr<StatementNode>>, Comparator> p_queue;
	queue<shared_ptr<StatementNode>> queue;
	queue.push(node);
	p_queue.push(node);
	while (!queue.empty()) {
		shared_ptr<StatementNode> current_node = queue.front();
		queue.pop();
		if (current_node->getNodeType() == NodeType::While) {
			auto inner_nodes = processLoopEntry(current_node).first;
			if (node->getNodeRef() > current_node->getNodeRef() && any_of(inner_nodes.begin(), inner_nodes.end(), [&](const auto &inner) {
					return inner->getNodeRef() >= node->getNodeRef();
				})) {
				p_queue = std::priority_queue<shared_ptr<StatementNode>, vector<shared_ptr<StatementNode>>, Comparator>();
				p_queue.push(current_node);
				queue = std::queue<shared_ptr<StatementNode>>();
			}
			auto external = (this->*info.loop_continuation_handler)(current_node).second;
			for (const auto &external_node : external) {
				p_queue.push(external_node);
				queue.push(external_node);
			}
			continue;
		}
		auto current_set = (*current_node.*info.gatherer)();
		for (const auto &neighbour : current_set) {
			if (neighbour->getNodeType() == NodeType::Dummy) {
				handleDummyNodeSearch(queue, neighbour, info.collector);
				handleDummyNodeSearch(p_queue, neighbour, info.collector);
				continue;
			}
			shared_ptr<StatementNode> neighbour_statement = dynamic_pointer_cast<StatementNode>(neighbour);
			p_queue.push(neighbour_statement);
			queue.push(neighbour_statement);
		}
	}
	return p_queue;
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
		auto cached_set = info.cache->find(statement->getNodeRef())->second;
		star.insert(cached_set.begin(), cached_set.end());
	}
	info.cache->insert({node->getNodeRef(), star});
}

void PKB::NextManager::processLoopNode(const shared_ptr<StatementNode> &node, TraversalInformation &info) {
	auto loop_nodes = traverseLoop(node);
	auto combined = loop_nodes;
	unordered_set<shared_ptr<StatementNode>> subsequent_nodes = ((this->*info.loop_continuation_handler)(node)).second;
	if (!subsequent_nodes.empty()) {
		for (const auto &subsequent : subsequent_nodes) {
			auto subsequent_next_star = info.cache->find(subsequent->getNodeRef())->second;
			combined.merge(subsequent_next_star);
			combined.insert(subsequent->getStmtInfo());
		}
	}
	for (const auto &internal_node : loop_nodes) {
		info.cache->insert({internal_node->getIdentifier(), combined});
	}
}

StmtInfoPtrSet PKB::NextManager::traverseLoop(const shared_ptr<NodeInterface> &node) {
	assert(node->getNodeType() == NodeType::While);
	shared_ptr<StatementNode> loop_node = dynamic_pointer_cast<StatementNode>(node);
	StmtInfoPtrSet set;
	queue<shared_ptr<NodeInterface>> queue;
	set.insert(loop_node->getStmtInfo());
	queue.push(*processLoopExit(loop_node).first.begin());
	while (!queue.empty()) {
		shared_ptr<NodeInterface> current = queue.front();
		queue.pop();
		if (current->getNodeType() == NodeType::Dummy) {
			handleDummyNodeSearch(queue, current, &ControlFlowGraph::collectNextOfDummy);
			continue;
		}
		shared_ptr<StatementNode> current_statement = dynamic_pointer_cast<StatementNode>(current);
		set.insert(current_statement->getStmtInfo());
		auto next_set = current_statement->getNextNodes();
		for (const auto &next : next_set) {
			if (next->getNodeType() != NodeType::Dummy) {
				auto xxx = dynamic_pointer_cast<StatementNode>(next);
				if (set.find(xxx->getStmtInfo()) != set.end()) {
					continue;
				}
			}
			queue.push(next);
		}
	}
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

template <typename Comparator>
pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> PKB::NextManager::processLoopEntryExit(
	const shared_ptr<PKB::StatementNode> &node, unordered_set<shared_ptr<PKB::NodeInterface>> (NodeInterface::*gatherer)() const,
	StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	assert(node->getNodeType() == PKB::NodeType::While);
	unordered_set<shared_ptr<PKB::NodeInterface>> node_set = (*node.*gatherer)();
	assert(!node_set.empty());
	if (node_set.size() == 1) {
		assert(dynamic_pointer_cast<PKB::StatementNode>(*node_set.begin()) != nullptr);
		return {{dynamic_pointer_cast<PKB::StatementNode>(*node_set.begin())}, {}};
	}
	auto first_node = *(node_set.begin());
	unordered_set<shared_ptr<PKB::StatementNode>> first_set;
	if (first_node->getNodeType() == PKB::NodeType::Dummy) {
		handleDummyNodeSearch(first_set, first_node, collector);
	} else {
		assert(dynamic_pointer_cast<PKB::StatementNode>(first_node) != nullptr);
		first_set.insert(dynamic_pointer_cast<StatementNode>(first_node));
	}
	auto second_node = *(++node_set.begin());
	unordered_set<shared_ptr<PKB::StatementNode>> second_set;
	if (second_node->getNodeType() == PKB::NodeType::Dummy) {
		handleDummyNodeSearch(second_set, second_node, collector);
	} else {
		assert(dynamic_pointer_cast<PKB::StatementNode>(second_node) != nullptr);
		second_set.insert(dynamic_pointer_cast<StatementNode>(second_node));
	}
	if (first_set.empty() ||
	    (!second_set.empty() && Comparator{}(second_set.begin()->get()->getNodeRef(), first_set.begin()->get()->getNodeRef()))) {
		return {second_set, first_set};
	}
	return {first_set, second_set};
}

pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> PKB::NextManager::processLoopExit(
	const shared_ptr<PKB::StatementNode> &node) {
	auto result = processLoopEntryExit<std::less<StmtRef>>(node, &NodeInterface::getNextNodes, &ControlFlowGraph::collectNextOfDummy);
	assert(result.first.size() == 1);
	assert(result.second.size() < 2);
	return result;
}

pair<unordered_set<shared_ptr<PKB::StatementNode>>, unordered_set<shared_ptr<PKB::StatementNode>>> PKB::NextManager::processLoopEntry(
	const shared_ptr<PKB::StatementNode> &node) {
	return processLoopEntryExit<std::greater<StmtRef>>(node, &NodeInterface::getPreviousNodes, &ControlFlowGraph::collectPreviousOfDummy);
}
