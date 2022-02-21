#include "QueryGraph.h"

#include <queue>

QP::QueryGraph::QueryGraph(const DeclarationList& declarations) {
	for (const Declaration& declaration : declarations) {
		Node node = {declaration.symbol, {}};
		nodes.insert({declaration.symbol, node});
	}
}

void QP::QueryGraph::setEdges(const SuchThatClauseList& such_that_clause_list, const PatternClauseList& pattern_clause_list) {
	for (const SuchThatClause& such_that_clause : such_that_clause_list) {
		vector<string> declarations = such_that_clause.relation->getDeclarationSymbols();
		if (declarations.size() == 2) {
			addEdge(declarations[0], declarations[1]);
		}
	}

	for (const PatternClause& pattern_clause : pattern_clause_list) {
		vector<string> declarations = pattern_clause.relation->getDeclarationSymbols();
		if (declarations.size() == 2) {
			addEdge(declarations[0], declarations[1]);
		}
	}
}

unordered_map<string, Node> QP::QueryGraph::getNodes() { return nodes; }

void QP::QueryGraph::addEdge(const string& symbol_one, const string& symbol_two) {
	Node node_one = this->nodes.at(symbol_one);
	if (find(node_one.adjacent_symbols.begin(), node_one.adjacent_symbols.end(), symbol_two) == node_one.adjacent_symbols.end()) {
		node_one.adjacent_symbols.push_back(symbol_two);
		nodes[symbol_one] = node_one;
	}

	Node node_two = this->nodes.at(symbol_two);
	if (find(node_two.adjacent_symbols.begin(), node_two.adjacent_symbols.end(), symbol_one) == node_two.adjacent_symbols.end()) {
		node_two.adjacent_symbols.push_back(symbol_one);
		nodes[symbol_two] = node_two;
	}
}

vector<unordered_set<string>> QP::QueryGraph::getSynonymsInGroup(const string& selected_synonym) {
	// Run BFS on the selected node
	vector<unordered_set<string>> result;
	unordered_set<string> group;
	unordered_set<string> unvisited_nodes;

	for (auto& node : nodes) {
		unvisited_nodes.insert(node.first);
	}

	queue<string> queue;
	queue.push(selected_synonym);

	while (!queue.empty()) {
		string symbol = queue.front();
		group.insert(symbol);
		unvisited_nodes.erase(symbol);
		queue.pop();

		Node node = this->nodes.at(symbol);
		for (const string& adjacent_symbol : node.adjacent_symbols) {
			if (unvisited_nodes.find(adjacent_symbol) != unvisited_nodes.end()) {
				queue.push(adjacent_symbol);
			}
		}

		// If queue is empty but there are still unvisited nodes,
		// we add an unvisited node to queue
		if (queue.empty() && !unvisited_nodes.empty()) {
			queue.push(*unvisited_nodes.begin());
			result.push_back(group);
			group.clear();
		}
	}

	result.push_back(group);
	return result;
}
