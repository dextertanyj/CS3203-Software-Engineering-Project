#include "PKB/AffectsManager.h"

#include <cassert>

PKB::AffectsManager::AffectsManager(ControlFlowGraph& control_flow_graph, SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
                                    SVRelationStore<PKB::UsesSRelation>& uses_store)
	: control_flow_graph(control_flow_graph), uses_store(uses_store), modifies_store(modifies_store) {}

bool PKB::AffectsManager::checkAffects(StmtRef first, StmtRef second) {
	if (!control_flow_graph.contains(first) || !control_flow_graph.contains(second)) {
		return false;
	}
	StmtInfoPtrSet affected_nodes = getAffects(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [&](const shared_ptr<StmtInfo>& info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffects(StmtRef first) {
	if (!control_flow_graph.contains(first)) {
		return {};
	}

	if (affects_cache.find(first) != affects_cache.end()) {
		return affects_cache.at(first);
	}

	if (control_flow_graph.getType(first) != StmtType::Assign) {
		return {};
	}

	VarRef variable = *(modifies_store.getByStmt(first).begin());
	DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto& neighbour : control_flow_graph.getNextNodes(first)) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &AffectsManager::processNodeAffects);
	}

	affects_cache.insert({first, info.nodes});
	return info.nodes;
}

StmtInfoPtrSet PKB::AffectsManager::getAffected(StmtRef second) {
	if (!control_flow_graph.contains(second)) {
		return {};
	}

	if (affected_cache.find(second) != affected_cache.end()) {
		return affected_cache.at(second);
	}

	if (control_flow_graph.getType(second) != StmtType::Assign) {
		return {};
	}

	VarRefSet variables = uses_store.getByStmt(second);
	StmtInfoPtrSet affected_set;
	for (const string& variable : variables) {
		StmtInfoPtrSet affected = getAffectedLoop(second, variable);
		affected_set.insert(affected.begin(), affected.end());
	}

	affected_cache.insert({second, affected_set});
	return affected_set;
}

StmtInfoPtrSet PKB::AffectsManager::getAffectedLoop(StmtRef node, VarRef variable) const {
	DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto& neighbour : control_flow_graph.getPreviousNodes(node)) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &AffectsManager::processNodeAffected);
	}
	return info.nodes;
}

void PKB::AffectsManager::processDFSVisit(DFSInfo& info,
                                          void (AffectsManager::*processor)(DFSInfo&, const shared_ptr<StmtInfo>&) const) const {
	shared_ptr<StmtInfo> current = info.node_stack.top();
	info.node_stack.pop();
	if (info.visited_set.find(current) != info.visited_set.end()) {
		return;
	}
	info.visited_set.insert(current);
	(this->*processor)(info, current);
}

void PKB::AffectsManager::processNodeAffects(DFSInfo& info, const shared_ptr<StmtInfo>& current) const {
	auto current_idx = current->getIdentifier();
	auto current_type = current->getType();
	if (uses_store.check(current_idx, info.variable) && current_type == StmtType::Assign) {
		info.nodes.insert(current);
	}

	// If the current statement is a container, its modifies consist only of the modifies of statements within it.
	bool is_container = current_type == StmtType::WhileStmt || current_type == StmtType::If;
	if (!is_container && modifies_store.check(current_idx, info.variable)) {
		return;
	}
	for (const auto& neighbour : control_flow_graph.getNextNodes(current_idx)) {
		info.node_stack.push(neighbour);
	}
}

void PKB::AffectsManager::processNodeAffected(DFSInfo& info, const shared_ptr<StmtInfo>& current) const {
	auto current_idx = current->getIdentifier();
	auto current_type = current->getType();
	bool is_container = current_type == StmtType::WhileStmt || current_type == StmtType::If;

	// If the current statement is a container, its modifies consist only of the modifies of statements within it.
	if (!is_container && modifies_store.check(current_idx, info.variable)) {
		if (current->getType() == StmtType::Assign) {
			info.nodes.insert(current);
		}
	} else {
		for (const auto& neighbour : control_flow_graph.getPreviousNodes(current_idx)) {
			info.node_stack.push(neighbour);
		}
	}
}

bool PKB::AffectsManager::checkAffectsStar(StmtRef first, StmtRef second) {
	if (!control_flow_graph.contains(first) || !control_flow_graph.contains(second)) {
		return false;
	}
	StmtInfoPtrSet affected_nodes = getAffectsStar(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [&](const shared_ptr<StmtInfo>& info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffectsStar(StmtRef node_ref) {
	if (!control_flow_graph.contains(node_ref)) {
		return {};
	}

	if (affects_star_cache.find(node_ref) != affects_star_cache.end()) {
		return affects_star_cache.at(node_ref);
	}
	if (cache_graph_store.find(node_ref) == cache_graph_store.end()) {
		auto graph_index = control_flow_graph.getGraphIndex(node_ref);
		buildCacheGraph(graph_index);
	}
	if (control_flow_graph.getType(node_ref) != StmtType::Assign) {
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
	if (!control_flow_graph.contains(node_ref)) {
		return {};
	}

	if (affected_star_cache.find(node_ref) != affected_star_cache.end()) {
		return affected_star_cache.at(node_ref);
	}
	if (cache_graph_store.find(node_ref) == cache_graph_store.end()) {
		auto graph_index = control_flow_graph.getGraphIndex(node_ref);
		buildCacheGraph(graph_index);
	}
	if (control_flow_graph.getType(node_ref) != StmtType::Assign) {
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
	while (!stack.empty()) {
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
		auto info = control_flow_graph.getStatementInfo(i);
		auto affected_set = iter->second;
		for (const auto& affected : affected_set) {
			affected_cache[affected->getIdentifier()].insert(info);
		}
	}
}

void PKB::AffectsManager::buildCacheGraphForwardVisit(StmtRef index, StmtRefSet& visited, stack<StmtRef>& stack) const {
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

StmtInfoPtrSet PKB::AffectsManager::buildCacheGraphReverseVisit(StmtRef index, StmtRefSet& visited) const {
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

void PKB::AffectsManager::processComponent(StmtRef index, StmtInfoPtrSet component) {
	auto self = control_flow_graph.getStatementInfo(index);
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

void PKB::AffectsManager::connectRelevantComponents(StmtRef index) {
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
