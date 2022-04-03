#include "PKB/AffectsManager.h"

#include <cassert>

PKB::AffectsManager::AffectsManager(ControlFlowGraph& control_flow_graph,
                                    SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
                                    SVRelationStore<PKB::UsesSRelation>& uses_store)
	: control_flow_graph(control_flow_graph), uses_store(uses_store), modifies_store(modifies_store) {}

bool PKB::AffectsManager::checkAffects(StmtRef first, StmtRef second) {
	StmtInfoPtrSet affected_nodes = getAffects(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [&](const shared_ptr<StmtInfo>& info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffects(StmtRef first) {
	if (affects_cache.find(first) != affects_cache.end()) {
		return affects_cache.at(first);
	}

	shared_ptr<PKB::StatementNode> start_node = this->control_flow_graph.getNode(first);
	if (start_node->getStmtInfo()->getType() != StmtType::Assign) {
		return {};
	}

	VarRef variable = *(modifies_store.getByStmt(first).begin());
	DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto& neighbour : start_node->getNextNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &ControlFlowGraph::collectNextOfDummy, &AffectsManager::processNodeAffects);
	}

	affects_cache.insert({first, info.nodes});
	return info.nodes;
}

StmtInfoPtrSet PKB::AffectsManager::getAffected(StmtRef second) {
	if (affected_cache.find(second) != affected_cache.end()) {
		return affected_cache.at(second);
	}

	shared_ptr<PKB::StatementNode> node = this->control_flow_graph.getNode(second);
	if (node->getStmtInfo()->getType() != StmtType::Assign) {
		return {};
	}
	VarRefSet variables = uses_store.getByStmt(second);
	StmtInfoPtrSet affected_set;
	for (const string& variable : variables) {
		StmtInfoPtrSet affected = getAffectedByNodeAndVar(node, variable);
		affected_set.insert(affected.begin(), affected.end());
	}

	affected_cache.insert({second, affected_set});
	return affected_set;
}

StmtInfoPtrSet PKB::AffectsManager::getAffectedByNodeAndVar(const shared_ptr<PKB::StatementNode>& node, VarRef variable) {
	DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto& neighbour : node->getPreviousNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &ControlFlowGraph::collectPreviousOfDummy, &AffectsManager::processNodeAffected);
	}
	return info.nodes;
}

void PKB::AffectsManager::processDFSVisit(DFSInfo& info, StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&),
                                          void (AffectsManager::*processor)(DFSInfo&, const shared_ptr<PKB::StatementNode>&)) {
	shared_ptr<PKB::NodeInterface> curr_node = info.node_stack.top();
	info.node_stack.pop();
	if (info.visited_set.find(curr_node) != info.visited_set.end()) {
		return;
	}
	if (curr_node->getNodeType() == NodeType::Dummy) {
		StmtInfoPtrSet real_nodes = (*collector)(curr_node);
		for (const auto& real_node : real_nodes) {
			info.node_stack.push(control_flow_graph.getNode(real_node->getIdentifier()));
		}
		return;
	}
	info.visited_set.insert(curr_node);
	shared_ptr<PKB::StatementNode> curr_stmt_node = dynamic_pointer_cast<PKB::StatementNode>(curr_node);
	(this->*processor)(info, curr_stmt_node);
}

void PKB::AffectsManager::processNodeAffects(DFSInfo& info, const shared_ptr<PKB::StatementNode>& curr_stmt_node) {
	if (uses_store.check(curr_stmt_node->getNodeRef(), info.variable) && curr_stmt_node->getStmtInfo()->getType() == StmtType::Assign) {
		info.nodes.insert(curr_stmt_node->getStmtInfo());
	}
	if (!modifies_store.check(curr_stmt_node->getNodeRef(), info.variable)) {
		for (const auto& neighbour : curr_stmt_node->getNextNodes()) {
			info.node_stack.push(neighbour);
		}
	}
}

void PKB::AffectsManager::processNodeAffected(DFSInfo& info, const shared_ptr<PKB::StatementNode>& curr_stmt_node) {
	if (modifies_store.check(curr_stmt_node->getNodeRef(), info.variable)) {
		if (curr_stmt_node->getStmtInfo()->getType() == StmtType::Assign) {
			info.nodes.insert(curr_stmt_node->getStmtInfo());
		}
	} else {
		for (const auto& neighbour : curr_stmt_node->getPreviousNodes()) {
			info.node_stack.push(neighbour);
		}
	}
}

bool PKB::AffectsManager::checkAffectsStar(StmtRef first, StmtRef second) {
	StmtInfoPtrSet affected_nodes = getAffectsStar(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [&](const shared_ptr<StmtInfo>& info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffectsStar(StmtRef node_ref) {
	if (affects_star_cache.find(node_ref) != affects_star_cache.end()) {
		return affects_star_cache.at(node_ref);
	}
	if (cache_graph_store.find(node_ref) == cache_graph_store.end()) {
		auto node = control_flow_graph.getNode(node_ref);
		buildCacheGraph(node->getGraphIndex());
	}
	if (control_flow_graph.getNode(node_ref)->getStmtInfo()->getType() != StmtType::Assign) {
		return {};
	}
	assert(cache_graph_store.find(node_ref) != cache_graph_store.end());
	auto node = cache_graph_store.at(node_ref);
	StmtInfoPtrSet result_set;
	if (node->strongly_connected) {
		result_set.insert(node->statements.begin(), node->statements.end());
	}
	for (const auto& graph_nodes : node->affects) {
		for (const auto& statements : graph_nodes->statements) {
			result_set.insert(statements);
			auto result = getAffectsStar(statements->getIdentifier());
			result_set.insert(result.begin(), result.end());
		}
	}
	affects_star_cache.insert({node_ref, result_set});
	return result_set;
}

StmtInfoPtrSet PKB::AffectsManager::getAffectedStar(StmtRef node_ref) {
	if (affected_star_cache.find(node_ref) != affected_star_cache.end()) {
		return affected_star_cache.at(node_ref);
	}
	if (cache_graph_store.find(node_ref) == cache_graph_store.end()) {
		auto node = control_flow_graph.getNode(node_ref);
		buildCacheGraph(node->getGraphIndex());
	}
	if (control_flow_graph.getNode(node_ref)->getStmtInfo()->getType() != StmtType::Assign) {
		return {};
	}
	assert(cache_graph_store.find(node_ref) != cache_graph_store.end());
	auto node = cache_graph_store.at(node_ref);
	StmtInfoPtrSet result_set;
	if (node->strongly_connected) {
		result_set.insert(node->statements.begin(), node->statements.end());
	}
	for (const auto& graph_nodes : node->affected) {
		for (const auto& statements : graph_nodes->statements) {
			result_set.insert(statements);
			auto result = getAffectedStar(statements->getIdentifier());
			result_set.insert(result.begin(), result.end());
		}
	}
	affected_star_cache.insert({node_ref, result_set});
	return result_set;
}

void PKB::AffectsManager::resetCache() {
	cache_graph_store.clear();
	affects_cache.clear();
	affects_star_cache.clear();
	affected_cache.clear();
	affected_star_cache.clear();
}

void PKB::AffectsManager::buildCacheGraph(size_t graph_index) {
	// Perform Kosaraju's algorithm to build a DAG of Affects* relationships.
	StmtRef start = control_flow_graph.getFirstIndex(graph_index);
	StmtRef end = control_flow_graph.getLastIndex(graph_index);
	computeAllAffects(start, end);
	transposeAffects(start, end);
	StmtRefSet visited;
	stack<StmtRef> stack;
	for (StmtRef i = start; i <= end; i++) {
		buildCacheGraphForwardVisit(i, visited, stack);
	}
	visited.clear();
	while(!stack.empty()) {
		StmtRef index = stack.top();
		stack.pop();
		if (visited.find(index) != visited.end()) {
			continue;
		}
		auto component = buildCacheGraphReverseVisit(index, visited);
		processComponent(index, move(component));
	}
	// Visited only contains assignment statements at this point,
	// so it is more efficient than using the statement index bounds.
	for (const auto& statement : visited) {
		connectRelevantComponents(statement);
	}
}

void PKB::AffectsManager::computeAllAffects(StmtRef start, StmtRef end) {
	for (StmtRef i = start; i <= end; i++) {
		getAffects(i);
	}
}

void PKB::AffectsManager::transposeAffects(StmtRef start, StmtRef end) {
	for (StmtRef i = start; i <= end; i++) {
		auto iter = affects_cache.find(i);
		if (iter == affects_cache.end()) {
			continue;
		}
		auto info = control_flow_graph.getNode(i)->getStmtInfo();
		auto affected_set = iter->second;
		for (const auto& affected : affected_set) {
			affected_cache[affected->getIdentifier()].insert(info);
		}
	}
}

void PKB::AffectsManager::buildCacheGraphForwardVisit(StmtRef index, StmtRefSet& visited, stack<StmtRef>& stack) {
	if (visited.find(index) != visited.end()) {
		return;
	}
	auto affects = affects_cache.find(index);
	// If the statement does not exist in the affects cache, then it is not an assignment statement.
	if (affects == affects_cache.end()) {
		return;
	}
	visited.insert(index);
	for (const auto& affect : affects->second) {
		buildCacheGraphForwardVisit(affect->getIdentifier(), visited, stack);
	}
	stack.push(index);
}

StmtInfoPtrSet PKB::AffectsManager::buildCacheGraphReverseVisit(StmtRef index, StmtRefSet& visited) {
	visited.insert(index);
	auto iter = affected_cache.find(index);
	if (iter == affected_cache.end()) {
		return {};
	}
	StmtInfoPtrSet result_set;
	for (const auto& affected : iter->second) {
		// To account for self-loops we allow a node to visit itself once more.
		auto affected_idx = affected->getIdentifier();
		if (visited.find(affected_idx) != visited.end() && affected_idx != index) {
			continue;
		}
		result_set.insert(affected);
		// We avoid visiting a self loop.
		if (affected_idx == index) {
			continue;
		}
		auto result = buildCacheGraphReverseVisit(affected->getIdentifier(), visited);
		result_set.insert(result.begin(), result.end());
	}
	return result_set;
}

void PKB::AffectsManager::processComponent(const StmtRef& index, StmtInfoPtrSet component) {
	auto self = control_flow_graph.getNode(index)->getStmtInfo();
	if (component.empty()) {
		auto node = make_shared<CacheGraphNode>(StmtInfoPtrSet{self}, false);
		cache_graph_store.insert({self->getIdentifier(), node});
	} else {
		component.insert(self);
		auto node = make_shared<CacheGraphNode>(component, true);
		for (const auto& statement : component) {
			cache_graph_store.insert({statement->getIdentifier(), node});
		}
	}
}

void PKB::AffectsManager::connectRelevantComponents(const StmtRef& index) {
	auto current_node = cache_graph_store.at(index);
	auto affects = affects_cache.at(index);
	for (const auto& affect : affects) {
		auto affected_node = cache_graph_store.at(affect->getIdentifier());
		if (affected_node == current_node) {
			continue;
		}
		current_node->affects.insert(affected_node);
		affected_node->affected.insert(current_node);
	}
}
