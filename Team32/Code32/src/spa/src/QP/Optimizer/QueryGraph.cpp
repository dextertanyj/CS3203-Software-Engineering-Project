#include "QP/Optimizer/QueryGraph.h"

#include <cassert>
#include <climits>

#define SINK_NODE_SYMBOL ""

using namespace std;
using namespace QP::Evaluator;
using namespace QP::Optimizer;
using namespace QP::Types;

QueryGraph::QueryGraph(const DeclarationList& declarations, const ClauseList& clauses, const DeclarationList& select_list) {
	nodes.reserve(declarations.size());
	for (const Declaration& declaration : declarations) {
		Node node = {declaration.symbol, {}, {}, SIZE_MAX};
		nodes.emplace(declaration.symbol, node);
	}
	setEdges(clauses);
	optimize(select_list);
}

unordered_map<string, QueryGraph::Node> QueryGraph::getNodes() const { return nodes; }

size_t QueryGraph::getNumberOfGroups() const { return clause_groups.getNumberOfGroups(); }

vector<string> QueryGraph::getGroupSynonyms(size_t group_number) const { return clause_groups.getGroupSynonyms(group_number); }

DeclarationList QueryGraph::getGroupSelectedSynonyms(size_t group_number) const {
	return clause_groups.getGroupSelectedSynonyms(group_number);
}

ClauseList QueryGraph::getGroupClauses(size_t group_number) const {
	ClauseList clauses;

	priority_queue<Edge, vector<Edge>, EdgeComparator> priority_queue;

	unordered_set<string> visited_nodes;
	const Node& cheapest_node = nodes.at(getCheapestNodeInGroup(group_number));
	visited_nodes.emplace(cheapest_node.declaration_symbol);
	for (const auto& edge : cheapest_node.outgoing_edges) {
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

void QueryGraph::setEdges(const ClauseList& clause_list) {
	for (const auto& clause : clause_list) {
		setEdge(clause);
	}
}

void QueryGraph::setEdge(const shared_ptr<Clause>& clause) {
	vector<string> declarations = clause->getDeclarationSymbols();
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

void QueryGraph::addEdge(const pair<string, string>& symbols, const shared_ptr<Clause>& clause) {
	Node& node = this->nodes.at(symbols.first);
	node.adjacent_symbols.emplace(symbols.second);
	size_t edge_weight = clause->getCost();
	node.outgoing_edges.push_back({symbols.first, symbols.second, clause, edge_weight});

	if (edge_weight < node.weight) {
		node.weight = edge_weight;
	}
}

void QueryGraph::optimize(const DeclarationList& select_list) {
	if (nodes.empty()) {
		return;
	}

	unordered_set<string> unvisited_nodes;
	unordered_map<string, Declaration> selected_nodes;
	unvisited_nodes.reserve(nodes.size());
	selected_nodes.reserve(select_list.size());

	for (const auto& node : nodes) {
		unvisited_nodes.emplace(node.first);
	}

	for (const Declaration& declaration : select_list) {
		selected_nodes.emplace(declaration.symbol, declaration);
	}

	traverseGraph(unvisited_nodes, selected_nodes);

	clause_groups.sort();
}

void QueryGraph::traverseGraph(unordered_set<string>& unvisited_nodes, unordered_map<string, Declaration>& selected_nodes) {
	queue<string> queue;
	vector<string> current_synonyms;
	DeclarationList current_selected;
	unsigned long long current_cost = 0;
	queue.push(*unvisited_nodes.begin());

	while (!queue.empty()) {
		string symbol = queue.front();
		queue.pop();
		unvisited_nodes.erase(symbol);
		current_synonyms.push_back(symbol);

		if (selected_nodes.find(symbol) != selected_nodes.end()) {
			current_selected.push_back(selected_nodes[symbol]);
			// We can safely erase the symbol from selected nodes since each symbol appears in only one connected component.
			selected_nodes.erase(symbol);
		}

		Node node = this->nodes.at(symbol);
		addNodeToQueue(node, queue, unvisited_nodes, current_cost);

		if (queue.empty()) {
			clause_groups.insertGroup(current_cost, current_synonyms, current_selected);
			current_cost = 0;
			current_synonyms.clear();
			current_selected.clear();
		}

		if (queue.empty() && !unvisited_nodes.empty()) {
			queue.push(*unvisited_nodes.begin());
		}
	}
}

void QueryGraph::addNodeToQueue(const Node& node, queue<string>& queue, unordered_set<string>& unvisited_nodes, unsigned long long& cost) {
	for (const string& adjacent_symbol : node.adjacent_symbols) {
		if (unvisited_nodes.find(adjacent_symbol) != unvisited_nodes.end()) {
			queue.push(adjacent_symbol);
			unvisited_nodes.erase(adjacent_symbol);
		}
	}
	for (const auto& edge : node.outgoing_edges) {
		cost += edge.weight;
	}
}

void QueryGraph::insertEdgesToQueue(unordered_set<string>& visited_nodes, const string& node_symbol,
                                    priority_queue<Edge, vector<Edge>, EdgeComparator>& pq) const {
	visited_nodes.emplace(node_symbol);
	const Node& node = nodes.at(node_symbol);
	for (const auto& new_edge : node.outgoing_edges) {
		if (visited_nodes.find(new_edge.node_to_symbol) == visited_nodes.end()) {
			pq.push(new_edge);
		}
	}
}

string QueryGraph::getCheapestNodeInGroup(size_t group_number) const {
	assert(clause_groups.getNumberOfGroups() != 0);

	auto group_synonyms = clause_groups.getGroupSynonyms(group_number);

	assert(!group_synonyms.empty());

	string node_symbol = *group_synonyms.begin();
	size_t cheapest_value = nodes.at(node_symbol).weight;

	for (const auto& symbol : group_synonyms) {
		const Node& node = nodes.at(symbol);
		if (cheapest_value > node.weight) {
			node_symbol = node.declaration_symbol;
			cheapest_value = node.weight;
		}
	}

	return node_symbol;
}
