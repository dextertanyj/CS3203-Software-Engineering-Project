#include "QueryGraph.h"

#include <queue>
#include <vector>

#define DUMMY_NODE_SYMBOL ""

using QP::Types::Clause;
using QP::Types::ConnectedSynonyms;
using QP::Types::Declaration;

QP::QueryGraph::QueryGraph(const DeclarationList& declarations) {
	for (const Declaration& declaration : declarations) {
		Node node = {declaration.symbol, {}, {}, SIZE_MAX};
		nodes.insert({declaration.symbol, node});
	}
}

void QP::QueryGraph::setEdges(const ClauseList& clause_list) {
	for (const Clause& clause : clause_list) {
		setEdge(clause);
	}
}

unordered_map<string, Node> QP::QueryGraph::getNodes() { return nodes; }

void QP::QueryGraph::setEdge(const Clause& clause) {
	vector<string> declarations = clause.relation->getDeclarationSymbols();
	if (declarations.size() == 1) {
		addEdge({declarations[0], DUMMY_NODE_SYMBOL}, clause);
	}

	if (declarations.size() == 2) {
		addEdge({declarations[0], declarations[1]}, clause);
		addEdge({declarations[1], declarations[0]}, clause);
	}
}

void QP::QueryGraph::addEdge(const pair<string, string>& symbols, const Clause& clause) {
	Node& node = this->nodes.at(symbols.first);
	node.adjacent_symbols.insert(symbols.second);
	size_t edge_weight = clause.relation->getCost();
	node.connectingEdges.push_back({symbols.first, symbols.second, clause, edge_weight});

	if (edge_weight < node.weight) {
		node.weight = edge_weight;
	}
}

ConnectedSynonyms QP::QueryGraph::getConnectedSynonyms(const DeclarationList& select_list) {
	if (nodes.empty()) {
		return {};
	}

	ConnectedSynonyms connected_synonyms = ConnectedSynonyms();
	DeclarationList selected_declarations;
	unordered_set<string> unvisited_nodes;
	unordered_map<string, Declaration> selected_nodes;
	size_t group_number = 0;

	for (auto& node : nodes) {
		unvisited_nodes.insert(node.first);
	}

	for (const Declaration& declaration : select_list) {
		selected_nodes.insert({declaration.symbol, declaration});
	}

	queue<string> queue;
	queue.push(*unvisited_nodes.begin());

	while (!queue.empty()) {
		string symbol = queue.front();
		connected_synonyms.insertSynonym(symbol, group_number);
		if (selected_nodes.find(symbol) != selected_nodes.end()) {
			selected_declarations.push_back(selected_nodes[symbol]);
			selected_nodes.erase(symbol);
		}
		unvisited_nodes.erase(symbol);
		queue.pop();

		Types::Node node = this->nodes.at(symbol);
		for (const string& adjacent_symbol : node.adjacent_symbols) {
			if (unvisited_nodes.find(adjacent_symbol) != unvisited_nodes.end()) {
				queue.push(adjacent_symbol);
			}
		}

		if (queue.empty()) {
			connected_synonyms.insertSelectedDeclarations(group_number, selected_declarations);
			selected_declarations.clear();
			group_number++;

			if (!unvisited_nodes.empty()) {
				queue.push(*unvisited_nodes.begin());
			}
		}
	}

	connected_synonyms.setNumberOfGroups(group_number);
	this->connected_synonyms = connected_synonyms;
	return connected_synonyms;
}

ClauseList QP::QueryGraph::sortGroup(size_t group_number) {
	ClauseList clauses;

	priority_queue<Edge, vector<Edge>, QP::Types::EdgeComp> pq;

	unordered_set<string> visited_nodes;
	Node cheapest_node = nodes[getCheapestNodeInGroup(group_number)];
	visited_nodes.insert(cheapest_node.declaration_symbol);
	for (auto const& edge : cheapest_node.connectingEdges) {
		pq.push(edge);
	}

	while (!pq.empty()) {
		Edge edge = pq.top();
		pq.pop();
		if (edge.node_from_symbol != DUMMY_NODE_SYMBOL && visited_nodes.find(edge.node_from_symbol) == visited_nodes.end()) {
			insertEdgesToQueue(visited_nodes, edge.node_from_symbol, pq);
		}
		if (edge.node_to_symbol != DUMMY_NODE_SYMBOL && visited_nodes.find(edge.node_to_symbol) == visited_nodes.end()) {
			insertEdgesToQueue(visited_nodes, edge.node_to_symbol, pq);
		}

		clauses.push_back(edge.clause);
	}

	return clauses;
}

void QP::QueryGraph::insertEdgesToQueue(unordered_set<string>& visited_nodes, string node_symbol,
                                        priority_queue<Edge, vector<Edge>, QP::Types::EdgeComp>& pq) {
	visited_nodes.insert(node_symbol);
	Node& node = nodes[node_symbol];
	for (auto const& new_edge : node.connectingEdges) {
		if (visited_nodes.find(new_edge.node_to_symbol) == visited_nodes.end()) {
			pq.push(new_edge);
		}
	}
}

string QP::QueryGraph::getCheapestNodeInGroup(size_t group_number) {
	string node_symbol;
	size_t cheapest_value = SIZE_MAX;

	for (auto const& pair : nodes) {
		const Node& node = pair.second;
		if (connected_synonyms.getGroupNumber(node.declaration_symbol) != group_number) {
			continue;
		}

		if (cheapest_value > node.weight) {
			node_symbol = node.declaration_symbol;
			cheapest_value = node.weight;
		}
	}

	return node_symbol;
}
