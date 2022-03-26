#include "AffectsManager.h"

PKB::AffectsManager::AffectsManager(ControlFlowGraph &control_flow_graph, SVRelationStore<PKB::ModifiesSRelation> &modifies_store,
                                    SVRelationStore<PKB::UsesSRelation> &uses_store)
	: control_flow_graph(control_flow_graph), uses_store(uses_store), modifies_store(modifies_store) {}

bool PKB::AffectsManager::checkAffects(StmtRef first, StmtRef second) {
	StmtInfoPtrSet affected_nodes = getAffects(first);
	return any_of(affected_nodes.begin(), affected_nodes.end(),
	              [second](const shared_ptr<StmtInfo> &info) { return info->getIdentifier() == second; });
}

StmtInfoPtrSet PKB::AffectsManager::getAffects(StmtRef first) {
	shared_ptr<PKB::StatementNode> start_node = this->control_flow_graph.getNode(first);
	if (start_node->getStmtInfo()->getType() != StmtType::Assign) {
		throw invalid_argument("Affects statement must be an assign statement.");
	}
	VarRef variable = *(modifies_store.getByStmt(first).begin());
	Types::DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto &neighbour : start_node->getNextNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &ControlFlowGraph::collectNextOfDummy, &AffectsManager::processNodeAffects);
	}
	return info.nodes;
}

StmtInfoPtrSet PKB::AffectsManager::getAffected(StmtRef second) {
	shared_ptr<PKB::StatementNode> node = this->control_flow_graph.getNode(second);
	if (node->getStmtInfo()->getType() != StmtType::Assign) {
		throw invalid_argument("Affects statement must be an assign statement.");
	}
	VarRefSet variables = uses_store.getByStmt(second);
	StmtInfoPtrSet affected_set;
	for (const string &variable : variables) {
		StmtInfoPtrSet affected = getAffectedByNodeAndVar(node, variable);
		affected_set.insert(affected.begin(), affected.end());
	}
	return affected_set;
}

StmtInfoPtrSet PKB::AffectsManager::getAffectedByNodeAndVar(const shared_ptr<PKB::StatementNode> &node, VarRef variable) {
	Types::DFSInfo info = {std::move(variable), {}, {}, {}};
	for (const auto &neighbour : node->getPreviousNodes()) {
		info.node_stack.push(neighbour);
	}
	while (!info.node_stack.empty()) {
		processDFSVisit(info, &ControlFlowGraph::collectPreviousOfDummy, &AffectsManager::processNodeAffected);
	}
	return info.nodes;
}

void PKB::AffectsManager::processDFSVisit(Types::DFSInfo &info,
                                          StmtInfoPtrSet (ControlFlowGraph::*collector)(const shared_ptr<NodeInterface> &),
                                          void (AffectsManager::*processor)(Types::DFSInfo &, const shared_ptr<PKB::StatementNode> &)) {
	shared_ptr<PKB::NodeInterface> curr_node = info.node_stack.top();
	info.node_stack.pop();
	if (info.visited_set.find(curr_node) != info.visited_set.end()) {
		return;
	}
	if (curr_node->getNodeType() == NodeType::Dummy) {
		StmtInfoPtrSet real_nodes = (control_flow_graph.*collector)(curr_node);
		for (const auto &real_node : real_nodes) {
			info.node_stack.push(control_flow_graph.getNode(real_node->getIdentifier()));
		}
		return;
	}
	info.visited_set.insert(curr_node);
	shared_ptr<PKB::StatementNode> curr_stmt_node = dynamic_pointer_cast<PKB::StatementNode>(curr_node);
	(this->*processor)(info, curr_stmt_node);
}

void PKB::AffectsManager::processNodeAffects(PKB::Types::DFSInfo &info, const shared_ptr<PKB::StatementNode> &curr_stmt_node) {
	if (uses_store.check(curr_stmt_node->getNodeRef(), info.variable) && curr_stmt_node->getStmtInfo()->getType() == StmtType::Assign) {
		info.nodes.insert(curr_stmt_node->getStmtInfo());
	}
	if (!modifies_store.check(curr_stmt_node->getNodeRef(), info.variable)) {
		for (const auto &neighbour : curr_stmt_node->getNextNodes()) {
			info.node_stack.push(neighbour);
		}
	}
}

void PKB::AffectsManager::processNodeAffected(PKB::Types::DFSInfo &info, const shared_ptr<PKB::StatementNode> &curr_stmt_node) {
	if (modifies_store.check(curr_stmt_node->getNodeRef(), info.variable)) {
		if (curr_stmt_node->getStmtInfo()->getType() == StmtType::Assign) {
			info.nodes.insert(curr_stmt_node->getStmtInfo());
		}
	} else {
		for (const auto &neighbour : curr_stmt_node->getPreviousNodes()) {
			info.node_stack.push(neighbour);
		}
	}
}
