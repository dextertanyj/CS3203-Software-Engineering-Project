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

void QP::QueryGraph::setEdge(const shared_ptr<Relationship::Relation>& relation) {
	vector<string> declarations = relation->getDeclarationSymbols();
	if (declarations.size() == 2) {
		addEdge({declarations[0], declarations[1]});
		addEdge({declarations[1], declarations[0]});
	}
}

void QP::QueryGraph::addEdge(const pair<string, string>& symbols) {
	Node& node = this->nodes.at(symbols.first);
	if (find(node.adjacent_symbols.begin(), node.adjacent_symbols.end(), symbols.second) == node.adjacent_symbols.end()) {
		node.adjacent_symbols.push_back(symbols.second);
	}
}

unordered_map<string, int> QP::QueryGraph::getSynonymsInGroup(const string& selected_synonym) {
	// Run BFS on the selected node
	unordered_map<string, int> result;
	unordered_set<string> unvisited_nodes;
	int group_number = 0;

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
			group_number++;
		}
	}

	return result;
}
