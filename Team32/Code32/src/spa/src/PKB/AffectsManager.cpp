#include "PKB/AffectsManager.h"

#include <cassert>

PKB::AffectsManager::AffectsManager(ControlFlowGraph& control_flow_graph,
                                    SVRelationStore<PKB::ModifiesSRelation>& modifies_store,
                                    SVRelationStore<PKB::UsesSRelation>& uses_store)
	: control_flow_graph(&control_flow_graph), uses_store(&uses_store), modifies_store(&modifies_store) {}

bool PKB::AffectsManager::checkAffects(StmtRef first, StmtRef second) {
	StmtInfoPtrSet affected_nodes = getAffects(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [&](const shared_ptr<StmtInfo>& info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffects(StmtRef first) {
	if (affects_cache.find(first) != affects_cache.end()) {
		return affects_cache.at(first);
	}

	shared_ptr<PKB::StatementNode> start_node = this->control_flow_graph->getNode(first);
	if (start_node->getStmtInfo()->getType() != StmtType::Assign) {
		return {};
	}

	VarRef variable = *(modifies_store->getByStmt(first).begin());
	Types::DFSInfo info = {std::move(variable), {}, {}, {}};
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

	shared_ptr<PKB::StatementNode> node = this->control_flow_graph->getNode(second);
	if (node->getStmtInfo()->getType() != StmtType::Assign) {
		return {};
	}
	VarRefSet variables = uses_store->getByStmt(second);
	StmtInfoPtrSet affected_set;
	for (const string& variable : variables) {
		StmtInfoPtrSet affected = getAffectedByNodeAndVar(node, variable);
		affected_set.insert(affected.begin(), affected.end());
	}

	affected_cache.insert({second, affected_set});
	return affected_set;
}

StmtInfoPtrSet PKB::AffectsManager::getAffectedByNodeAndVar(const shared_ptr<PKB::StatementNode>& node, VarRef variable) {
	Types::DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto& neighbour : node->getPreviousNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &ControlFlowGraph::collectPreviousOfDummy, &AffectsManager::processNodeAffected);
	}
	return info.nodes;
}

void PKB::AffectsManager::processDFSVisit(Types::DFSInfo& info, StmtInfoPtrSet (*collector)(const shared_ptr<NodeInterface>&),
                                          void (AffectsManager::*processor)(Types::DFSInfo&, const shared_ptr<PKB::StatementNode>&)) {
	shared_ptr<PKB::NodeInterface> curr_node = info.node_stack.top();
	info.node_stack.pop();
	if (info.visited_set.find(curr_node) != info.visited_set.end()) {
		return;
	}
	if (curr_node->getNodeType() == NodeType::Dummy) {
		StmtInfoPtrSet real_nodes = (*collector)(curr_node);
		for (const auto& real_node : real_nodes) {
			info.node_stack.push(control_flow_graph->getNode(real_node->getIdentifier()));
		}
		return;
	}
	info.visited_set.insert(curr_node);
	shared_ptr<PKB::StatementNode> curr_stmt_node = dynamic_pointer_cast<PKB::StatementNode>(curr_node);
	(this->*processor)(info, curr_stmt_node);
}

void PKB::AffectsManager::processNodeAffects(PKB::Types::DFSInfo& info, const shared_ptr<PKB::StatementNode>& curr_stmt_node) {
	if (uses_store->check(curr_stmt_node->getNodeRef(), info.variable) && curr_stmt_node->getStmtInfo()->getType() == StmtType::Assign) {
		info.nodes.insert(curr_stmt_node->getStmtInfo());
	}
	if (!modifies_store->check(curr_stmt_node->getNodeRef(), info.variable)) {
		for (const auto& neighbour : curr_stmt_node->getNextNodes()) {
			info.node_stack.push(neighbour);
		}
	}
}

void PKB::AffectsManager::processNodeAffected(PKB::Types::DFSInfo& info, const shared_ptr<PKB::StatementNode>& curr_stmt_node) {
	if (modifies_store->check(curr_stmt_node->getNodeRef(), info.variable)) {
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
		auto node = control_flow_graph->getNode(node_ref);
		buildAffectsGraph(node->getGraphIndex());
	}
	assert(cache_graph_store.find(node_ref) != cache_graph_store.end());
	auto node = cache_graph_store.at(node_ref);
	StmtInfoPtrSet result_set;
	if (node->strongly_connected) {
		result_set.insert(node->statements.begin(), node->statements.end());
	}
	for (const auto& graph_nodes : node->affects) {
		for (const auto& statements : graph_nodes->statements) {
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
	queue<StmtRef> bfs_queue;
	bfs_queue.push(node_ref);
	Types::AffectStarBFSInfo info = {bfs_queue, {node_ref}, {}};
	while (!info.bfs_queue.empty()) {
		processAffectStarBFS(info, &AffectsManager::getAffected, affected_star_cache);
	}
	affected_star_cache.insert({node_ref, info.result});
	return info.result;
}

void PKB::AffectsManager::processAffectStarBFS(PKB::Types::AffectStarBFSInfo& info, StmtInfoPtrSet (AffectsManager::*gatherer)(StmtRef),
                                               unordered_map<StmtRef, StmtInfoPtrSet>& cache) {
	StmtRef current = info.bfs_queue.front();
	info.bfs_queue.pop();
	for (const auto& stmt : (this->*gatherer)(current)) {
		evaluateAffectStarBFSNode(stmt, info, cache);
	}
}

void PKB::AffectsManager::evaluateAffectStarBFSNode(const shared_ptr<StmtInfo>& stmt, Types::AffectsStarBFSInfo& info,
                                                    unordered_map<StmtRef, StmtInfoPtrSet>& cache) {
	info.result.insert(stmt);
	if (cache.find(stmt->getIdentifier()) != cache.end()) {
		info.visited_set.insert(stmt->getIdentifier());
		StmtInfoPtrSet affects_star_set_of_stmt = cache.at(stmt->getIdentifier());
		for (const auto& stmt_info : affects_star_set_of_stmt) {
			info.visited_set.insert(stmt_info->getIdentifier());
			info.result.insert(stmt_info);
		}
		return;
	}
	if (info.visited_set.find(stmt->getIdentifier()) == info.visited_set.end()) {
		info.visited_set.insert(stmt->getIdentifier());
		info.bfs_queue.push(stmt->getIdentifier());
	}
}

void PKB::AffectsManager::resetCache() {
	affects_cache.clear();
	affects_star_cache.clear();
	affected_cache.clear();
	affected_star_cache.clear();
}

pair<StmtRef, StmtRef> PKB::AffectsManager::computeAllAffects(size_t graph_index) {
	auto start = control_flow_graph->getStart(graph_index);
	auto ends = control_flow_graph->getEnd(graph_index);

	auto start_index = dynamic_pointer_cast<StatementNode>(start)->getNodeRef();
	auto end = ControlFlowGraph::collectPreviousOfDummy(ends);
	StmtRef end_index = 0;
	for (const auto& node : end) {
		end_index = std::max(end_index, node->getIdentifier());
	}
	for (StmtRef i = start_index; i <= end_index; i++) {
		getAffects(i);
	}
	return {start_index, end_index};
}

void PKB::AffectsManager::transposeAffects(StmtRef start, StmtRef end) {
	for (StmtRef i = start; i <= end; i++) {
		auto iter = affects_cache.find(i);
		if (iter == affects_cache.end()) {
			continue;
		}
		auto info = control_flow_graph->getNode(i)->getStmtInfo();
		auto affected_set = iter->second;
		for (const auto& affected : affected_set) {
			affected_cache[affected->getIdentifier()].insert(info);
		}
	}
}

void PKB::AffectsManager::buildAffectsGraph(size_t graph_index) {
	auto start_end = computeAllAffects(graph_index);
	StmtRef start = start_end.first;
	StmtRef end = start_end.second;
	transposeAffects(start, end);
	StmtRefSet visited;
	stack<StmtRef> stack;
	for (StmtRef i = start; i <= end; i++) {
		buildAffectsGraphVisit(i, visited, stack);
	}
	visited.clear();
	while(!stack.empty()) {
		StmtRef index = stack.top();
		stack.pop();
		if (visited.find(index) == visited.end()) {
			continue;
		}
		auto component = buildAffectsGraphReverseVisit(index, visited);
		auto self = control_flow_graph->getNode(index)->getStmtInfo();
		if (component.find(self) != component.end()) {
			auto node = make_shared<CacheGraphNode>(component, true);
			for (const auto& statement : component) {
				cache_graph_store.insert({statement->getIdentifier(), node});
			}
		} else {
			assert(component.size() == 1);
			auto node = make_shared<CacheGraphNode>(component, false);
			auto statement = *component.begin();
			cache_graph_store.insert({statement->getIdentifier(), node});
		}
	}
	// Visited only contains assignment statements at this point,
	// so it is more efficient than using the statement index bounds.
	for (const auto& statement : visited) {
		auto current_node = cache_graph_store.at(statement);
		auto affects = affects_cache.at(statement);
		for (const auto& affect : affects) {
			auto affected_node = cache_graph_store.at(affect->getIdentifier());
			current_node->affects.insert(affected_node);
			affected_node->affected.insert(current_node);
		}
	}
}

void PKB::AffectsManager::buildAffectsGraphVisit(StmtRef index, StmtRefSet& visited, stack<StmtRef>& stack) {
	if (visited.find(index) != visited.end()) {
		return;
	}
	auto affects = affects_cache.find(index);
	// If the statement does not exist in the affects cache, then it is not an assignment statement.
	if (affects == affects_cache.end()) {
		return;
	}
	for (const auto& affect : affects->second) {
		buildAffectsGraphVisit(affect->getIdentifier(), visited, stack);
	}
	stack.push(index);
}

StmtInfoPtrSet PKB::AffectsManager::buildAffectsGraphReverseVisit(StmtRef index, StmtRefSet& visited) {
	if (visited.find(index) != visited.end()) {
		return {};
	}
	visited.insert(index);
	auto iter = affected_cache.find(index);
	assert(iter != affected_cache.end());
	StmtInfoPtrSet result_set;
	for (const auto& affected	 : iter->second) {
		result_set.insert(affected);
		auto result = buildAffectsGraphReverseVisit(affected->getIdentifier(), visited);
		result_set.insert(result.begin(), result.end());
	}
	return result_set;
}
