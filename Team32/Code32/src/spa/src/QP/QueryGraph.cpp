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
		setEdge(such_that_clause.relation);
	}

	for (const PatternClause& pattern_clause : pattern_clause_list) {
		setEdge(pattern_clause.relation);
	}
}

unordered_map<string, Node> QP::QueryGraph::getNodes() { return nodes; }

void QP::QueryGraph::setEdge(shared_ptr<Relationship::Relation> relation) {
	vector<string> declarations = relation->getDeclarationSymbols();
	if (declarations.size() == 2) {
		addEdge(declarations[0], declarations[1]);
		addEdge(declarations[1], declarations[0]);
	}
}

void QP::QueryGraph::addEdge(const string& symbol_from, const string& symbol_to) {
	Node& node = this->nodes.at(symbol_from);
	if (find(node.adjacent_symbols.begin(), node.adjacent_symbols.end(), symbol_to) == node.adjacent_symbols.end()) {
		node.adjacent_symbols.push_back(symbol_to);
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
