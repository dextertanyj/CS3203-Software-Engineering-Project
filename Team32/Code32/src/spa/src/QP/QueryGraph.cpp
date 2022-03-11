#include "QueryGraph.h"

#include <queue>
#include <vector>

QP::QueryGraph::QueryGraph(const Types::DeclarationList& declarations) {
	for (const Types::Declaration& declaration : declarations) {
		Types::Node node = {declaration.symbol, {}};
		nodes.insert({declaration.symbol, node});
	}
}

void QP::QueryGraph::setEdges(const Types::ClauseList& clause_list) {
	for (const Types::Clause& clause : clause_list) {
		setEdge(clause.relation);
	}
}

unordered_map<string, QP::Types::Node> QP::QueryGraph::getNodes() { return nodes; }

void QP::QueryGraph::setEdge(const shared_ptr<Relationship::Relation>& relation) {
	vector<string> declarations = relation->getDeclarationSymbols();
	if (declarations.size() == 2) {
		addEdge({declarations[0], declarations[1]});
		addEdge({declarations[1], declarations[0]});
	}
}

void QP::QueryGraph::addEdge(const pair<string, string>& symbols) {
	Types::Node& node = this->nodes.at(symbols.first);
	if (find(node.adjacent_symbols.begin(), node.adjacent_symbols.end(), symbols.second) == node.adjacent_symbols.end()) {
		node.adjacent_symbols.push_back(symbols.second);
	}
}

unordered_map<string, size_t> QP::QueryGraph::getSynonymsInGroup(const string& selected_synonym) {
	// Run BFS on the selected node
	unordered_map<string, size_t> result;
	unordered_set<string> unvisited_nodes;
	size_t group_number = 0;

	for (auto& node : nodes) {
		unvisited_nodes.insert(node.first);
	}

	queue<string> queue;
	queue.push(selected_synonym);

	while (!queue.empty()) {
		string symbol = queue.front();
		result.insert({symbol, group_number});
		unvisited_nodes.erase(symbol);
		queue.pop();

		Types::Node node = this->nodes.at(symbol);
		for (const string& adjacent_symbol : node.adjacent_symbols) {
			if (unvisited_nodes.find(adjacent_symbol) != unvisited_nodes.end()) {
				queue.push(adjacent_symbol);
			}
		}

		// If queue is empty but there are still unvisited nodes,
		// we add an unvisited node to queue
		if (queue.empty() && !unvisited_nodes.empty()) {
			queue.push(*unvisited_nodes.begin());
			group_number++;
		}
	}

	return result;
}
