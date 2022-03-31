#include "QueryGraph.h"

#include <cassert>
#include <queue>
#include <vector>

#define SINK_NODE_SYMBOL ""

using QP::Types::Clause;
using QP::Types::ConnectedSynonyms;
using QP::Types::Declaration;

QP::QueryGraph::QueryGraph(const DeclarationList& declarations, const ClauseList& clauses, const DeclarationList& select_list) {
	for (const Declaration& declaration : declarations) {
		Node node = {declaration.symbol, {}, {}, SIZE_MAX};
		nodes.insert({declaration.symbol, node});
	}
	setEdges(clauses);
	optimize(select_list);
}

unordered_map<string, Node> QP::QueryGraph::getNodes() const { return nodes; }

size_t QP::QueryGraph::getNumberOfGroups() const { return connected_synonyms.getNumberOfGroups(); }

vector<string> QP::QueryGraph::getGroupSynonyms(size_t group_number) const { return connected_synonyms.getGroupSynonyms(group_number); }

DeclarationList QP::QueryGraph::getGroupSelectedSynonyms(size_t group_number) const {
	return connected_synonyms.getGroupSelectedSynonyms(group_number);
}

ClauseList QP::QueryGraph::getGroupClauses(size_t group_number) const {
	ClauseList clauses;

	priority_queue<Edge, vector<Edge>, EdgeComp> priority_queue;

	unordered_set<string> visited_nodes;
	const Node& cheapest_node = nodes.at(getCheapestNodeInGroup(group_number));
	visited_nodes.insert(cheapest_node.declaration_symbol);
	for (auto const& edge : cheapest_node.outgoing_edges) {
		priority_queue.push(edge);
	}

	while (!priority_queue.empty()) {
		Edge edge = priority_queue.top();
		priority_queue.pop();
		if (!edge.node_from_symbol.empty() && visited_nodes.find(edge.node_from_symbol) == visited_nodes.end()) {
			insertEdgesToQueue(visited_nodes, edge.node_from_symbol, priority_queue);
		}
		if (!edge.node_to_symbol.empty() && visited_nodes.find(edge.node_to_symbol) == visited_nodes.end()) {
			insertEdgesToQueue(visited_nodes, edge.node_to_symbol, priority_queue);
		}

		clauses.push_back(edge.clause);
	}

	return clauses;
}

void QP::QueryGraph::setEdges(const ClauseList& clause_list) {
	for (const Clause& clause : clause_list) {
		setEdge(clause);
	}
}

void QP::QueryGraph::setEdge(const Clause& clause) {
	vector<string> declarations = clause.relation->getDeclarationSymbols();
	if (declarations.empty()) {
		return;
	}

	// The sink node does not have any outgoing edges
	if (declarations.size() == 1 || declarations[0] == declarations[1]) {
		addEdge({declarations[0], SINK_NODE_SYMBOL}, clause);
		return;
	}

	addEdge({declarations[0], declarations[1]}, clause);
	addEdge({declarations[1], declarations[0]}, clause);
}

void QP::QueryGraph::addEdge(const pair<string, string>& symbols, const Clause& clause) {
	Node& node = this->nodes.at(symbols.first);
	node.adjacent_symbols.insert(symbols.second);
	size_t edge_weight = clause.relation->getCost();
	node.outgoing_edges.push_back({symbols.first, symbols.second, clause, edge_weight});

	if (edge_weight < node.weight) {
		node.weight = edge_weight;
	}
}

void QP::QueryGraph::optimize(const DeclarationList& select_list) {
	if (nodes.empty()) {
		return;
	}

	unordered_set<string> unvisited_nodes;
	unordered_map<string, Declaration> selected_nodes;
	unsigned long long current_cost = 0;
	vector<string> current_synonyms;
	DeclarationList current_selected;

	for (auto& node : nodes) {
		unvisited_nodes.insert(node.first);
	}

	for (const Declaration& declaration : select_list) {
		selected_nodes.insert({declaration.symbol, declaration});
	}

	queue<string> queue;
	auto start = *unvisited_nodes.begin();
	queue.push(start);
	unvisited_nodes.erase(start);

	while (!queue.empty()) {
		string symbol = queue.front();
		queue.pop();

		current_synonyms.push_back(symbol);

		if (selected_nodes.find(symbol) != selected_nodes.end()) {
			current_selected.push_back(selected_nodes[symbol]);
			// We can safely erase the symbol from selected nodes since each symbol appears in only one connected component.
			selected_nodes.erase(symbol);
		}

		Node node = this->nodes.at(symbol);
		addNodeToQueue(node, queue, unvisited_nodes, current_cost);

		if (queue.empty()) {
			connected_synonyms.insertGroup(current_cost, current_synonyms, current_selected);
			current_cost = 0;
			current_synonyms.clear();
			current_selected.clear();
		}

		if (queue.empty() && !unvisited_nodes.empty()) {
			start = *unvisited_nodes.begin();
			queue.push(start);
			unvisited_nodes.erase(start);
		}
	}
}

void QP::QueryGraph::addNodeToQueue(const Node& node, queue<string>& queue, unordered_set<string>& unvisited_nodes,
                                    unsigned long long& cost) {
	for (const string& adjacent_symbol : node.adjacent_symbols) {
		if (unvisited_nodes.find(adjacent_symbol) != unvisited_nodes.end()) {
			queue.push(adjacent_symbol);
		}
	}
	for (const auto& edge : node.outgoing_edges) {
		cost += edge.weight;
	}
}

void QP::QueryGraph::insertEdgesToQueue(unordered_set<string>& visited_nodes, const string& node_symbol,
                                        priority_queue<Edge, vector<Edge>, EdgeComp>& pq) const {
	visited_nodes.insert(node_symbol);
	const Node& node = nodes.at(node_symbol);
	for (auto const& new_edge : node.outgoing_edges) {
		if (visited_nodes.find(new_edge.node_to_symbol) == visited_nodes.end()) {
			pq.push(new_edge);
		}
	}
}

string QP::QueryGraph::getCheapestNodeInGroup(size_t group_number) const {
	assert(connected_synonyms.getNumberOfGroups() != 0);

	string node_symbol;
	size_t cheapest_value = SIZE_MAX;

	for (auto const& symbol : connected_synonyms.getGroupSynonyms(group_number)) {
		const Node& node = nodes.at(symbol);
		if (cheapest_value > node.weight) {
			node_symbol = node.declaration_symbol;
			cheapest_value = node.weight;
		}
	}

	return node_symbol;
}
