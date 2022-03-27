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

template <class T>
void PKB::NextManager::handleDummyNodeSearch(T &queue, const shared_ptr<NodeInterface> &dummy_node,
                                             StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	auto nodes = (control_flow_graph->*collector)(dummy_node);
	for (const auto &info : nodes) {
		shared_ptr<StatementNode> node = control_flow_graph->getNode(info->getIdentifier());
		queue.push(node);
	}
}

StmtInfoPtrSet PKB::NextManager::handleWhileNode(const shared_ptr<NodeInterface> &node) {
	assert(node->getNodeType() == NodeType::While);
	shared_ptr<StatementNode> while_node = dynamic_pointer_cast<StatementNode>(node);
	StmtInfoPtrSet set;
	queue<shared_ptr<NodeInterface>> queue;
	set.insert(while_node->getStmtInfo());
	auto yyy = while_node->getNextNodes();
	assert(yyy.size() > 0);
	if (yyy.size() == 1) {
		queue.push(dynamic_pointer_cast<StatementNode>(*yyy.begin()));
	} else {
		auto first_yy = *(yyy.begin());
		auto second_yy = *(++yyy.begin());
		if (first_yy->getNodeType() == NodeType::Dummy) {
			queue.push(dynamic_pointer_cast<StatementNode>(second_yy));
		} else if (second_yy->getNodeType() == NodeType::Dummy) {
			queue.push(dynamic_pointer_cast<StatementNode>(first_yy));
		} else {
			if (dynamic_pointer_cast<StatementNode>(first_yy)->getNodeRef() < dynamic_pointer_cast<StatementNode>(second_yy)->getNodeRef()) {
				queue.push(dynamic_pointer_cast<StatementNode>(first_yy));
			} else {
				queue.push(dynamic_pointer_cast<StatementNode>(second_yy));
			}
		}
	}
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
		for_each(next_set.begin(), next_set.end(), [&](const shared_ptr<NodeInterface> &next) {
			if (next->getNodeType() != NodeType::Dummy) {
				auto xxx = dynamic_pointer_cast<StatementNode>(next);
				if (set.find(xxx->getStmtInfo()) != set.end()) {
					return;
				}
			}
			queue.push(next);
		});
	}
	return set;
}

struct NextComparator {
	bool operator()(const shared_ptr<PKB::StatementNode> &lhs, const shared_ptr<PKB::StatementNode> &rhs) { return *lhs < *rhs; }
};

StmtInfoPtrSet PKB::NextManager::getNextStar(StmtRef node_ref) {
	if (next_cache.find(node_ref) != next_cache.end()) {
		return next_cache.find(node_ref)->second;
	}
	size_t graph_index = control_flow_graph->getNode(node_ref)->getGraphIndex();
	priority_queue<shared_ptr<StatementNode>, std::vector<shared_ptr<StatementNode>>, NextComparator> priority_queue;
	if (next_cache_metadata.find(graph_index) != next_cache_metadata.end()) {
		priority_queue.push(control_flow_graph->getNode(next_cache_metadata.find(graph_index)->second));
	} else if (control_flow_graph->getEnd(graph_index)->getNodeType() == NodeType::Dummy) {
		for (const auto &node : control_flow_graph->collectPreviousOfDummy(control_flow_graph->getEnd(graph_index))) {
			priority_queue.push(control_flow_graph->getNode(node->getIdentifier()));
		}
	} else {
		shared_ptr<StatementNode> end = dynamic_pointer_cast<StatementNode>(control_flow_graph->getEnd(graph_index));
		priority_queue.push(end);
	}
	while (!priority_queue.empty()) {
		shared_ptr<NodeInterface> node = priority_queue.top();
		priority_queue.pop();
		if (node->getNodeType() == NodeType::Dummy) {
			handleDummyNodeSearch(priority_queue, node, &ControlFlowGraph::collectPreviousOfDummy);
		}
		shared_ptr<StatementNode> current_node = dynamic_pointer_cast<StatementNode>(node);
		if (current_node->getNodeType() == NodeType::While) {
			auto loop_nodes = handleWhileNode(current_node);
			auto next = current_node->getNextNodes();
			auto combined = loop_nodes;
			assert(next.size() > 0);
			if (next.size() > 1) {
				shared_ptr<StmtInfo> true_next;
			shared_ptr<NodeInterface> first = *(next.begin());
			shared_ptr<NodeInterface> second = *(++next.begin());
			if (first->getNodeType() == NodeType::Dummy) {
				auto true_next_set = control_flow_graph->collectNextOfDummy(first);
				assert(true_next_set.size() == 1);
				true_next = *(true_next_set.begin());
			} else if (second->getNodeType() == NodeType::Dummy) {
				auto true_next_set = control_flow_graph->collectNextOfDummy(second);
				assert(true_next_set.size() == 1);
				true_next = *(true_next_set.begin());
			} else {
				shared_ptr<StatementNode> first_statement = dynamic_pointer_cast<StatementNode>(first);
				shared_ptr<StatementNode> second_statement = dynamic_pointer_cast<StatementNode>(second);
				true_next = first_statement->getNodeRef() > second_statement->getNodeRef() ? first_statement->getStmtInfo()
				                                                                           : second_statement->getStmtInfo();
			}
			auto xxxx = next_cache.find(true_next->getIdentifier())->second;
			combined.merge(xxxx);
			combined.insert(true_next);
			}
			for (const auto &internal_node : loop_nodes) {
				next_cache.insert({internal_node->getIdentifier(), combined});
			}
			auto previous = current_node->getPreviousNodes();
			unordered_set<shared_ptr<StatementNode>> first_previous_set;
			if ((previous.begin())->get()->getNodeType() == NodeType::Dummy) {
				auto previouses = control_flow_graph->collectPreviousOfDummy(*(previous.begin()));
				for (const auto &yyy : previouses) {
					first_previous_set.insert(control_flow_graph->getNode(yyy->getIdentifier()));
				}
			} else {
				first_previous_set.insert(dynamic_pointer_cast<StatementNode>(*previous.begin()));
			}
			unordered_set<shared_ptr<StatementNode>> second_previous_set;
			if (previous.size() == 2 && (++previous.begin())->get()->getNodeType() == NodeType::Dummy) {
				auto previouses = control_flow_graph->collectPreviousOfDummy(*(++previous.begin()));
				for (const auto &yyy : previouses) {
					second_previous_set.insert(control_flow_graph->getNode(yyy->getIdentifier()));
				}
			} else if (previous.size() == 2) {
				second_previous_set.insert(dynamic_pointer_cast<StatementNode>(*(++previous.begin())));
			}
			if (any_of(first_previous_set.begin(), first_previous_set.end(),
			           [&](const auto &xxx) { return xxx->getNodeRef() < current_node->getNodeRef(); })) {
				for_each(first_previous_set.begin(), first_previous_set.end(), [&](const auto &zzz) { priority_queue.push(zzz); });
			} else {
				for_each(second_previous_set.begin(), second_previous_set.end(), [&](const auto &zzz) { priority_queue.push(zzz); });
			}
			continue;
		}
		StmtInfoPtrSet next_star;
		unordered_set<shared_ptr<NodeInterface>> next_set = node->getNextNodes();
		while (!next_set.empty()) {
			shared_ptr<NodeInterface> next = *(next_set.begin());
			next_set.erase(next);
			if (next->getNodeType() == NodeType::Dummy) {
				auto x = control_flow_graph->collectNextOfDummy(next);
				for_each(x.begin(), x.end(), [&](const auto &yy) { next_set.insert(control_flow_graph->getNode(yy->getIdentifier())); });
				continue;
			}
			shared_ptr<StatementNode> statement = dynamic_pointer_cast<StatementNode>(next);
			next_star.insert(statement->getStmtInfo());
			auto cached_set = next_cache.find(statement->getNodeRef())->second;
			next_star.insert(cached_set.begin(), cached_set.end());
		}
		next_cache.insert({current_node->getNodeRef(), next_star});
		for (const auto &previous : node->getPreviousNodes()) {
			if (previous->getNodeType() == NodeType::Dummy) {
				auto yyy = control_flow_graph->collectPreviousOfDummy(previous);
				for (const auto &yss : yyy) {
					priority_queue.push(control_flow_graph->getNode(yss->getIdentifier()));
				}
			} else {
				priority_queue.push(dynamic_pointer_cast<StatementNode>(previous));
			}
		}
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
	Types::BFSInfo info = {control_flow_graph->getNode(node_ref), {}, {}, {}};
	info.node_queue.push(info.start_node);
	while (!info.node_queue.empty()) {
		shared_ptr<PKB::NodeInterface> curr_node = info.node_queue.front();
		info.node_queue.pop();
		unordered_set<shared_ptr<PKB::NodeInterface>> curr_prev_nodes = curr_node->getPreviousNodes();
		for (const auto &node : curr_prev_nodes) {
			processBFSVisit(info, node, &ControlFlowGraph::collectPreviousOfDummy);
		}
		info.visited_set.insert(curr_node);
	}
	return info.nodes;
}

void PKB::NextManager::processBFSVisit(Types::BFSInfo &info, const shared_ptr<PKB::NodeInterface> &node,
                                       StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &)) {
	if (node == info.start_node) {
		shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(node);
		info.nodes.insert(stmt_node->getStmtInfo());
	}
	if (info.visited_set.find(node) != info.visited_set.end()) {
		return;
	}
	if (node->getNodeType() == NodeType::Dummy) {
		StmtInfoPtrSet real_nodes = (control_flow_graph->*collector)(node);
		info.nodes.insert(real_nodes.begin(), real_nodes.end());
		for (const auto &real_node : real_nodes) {
			info.node_queue.push(control_flow_graph->getNode(real_node->getIdentifier()));
		}
	} else {
		shared_ptr<PKB::StatementNode> stmt_node = dynamic_pointer_cast<PKB::StatementNode>(node);
		info.nodes.insert(stmt_node->getStmtInfo());
		info.node_queue.push(node);
	}
}

void PKB::NextManager::resetCache() {
	next_cache.clear();
	previous_cache.clear();
}