#include "QueryGraph.h"

#include <queue>

QP::QueryGraph::QueryGraph(const DeclarationList& declarations) {
	for (const Declaration& declaration : declarations) {
		Node node = {declaration.symbol, {}};
		nodes.insert({declaration.symbol, node});
	}
}

void QP::QueryGraph::setEdges(const ClauseList& clause_list) {
	for (const Clause& clause : clause_list) {
		setEdge(clause.relation);
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

ConnectedSynonyms QP::QueryGraph::getConnectedSynonyms(const DeclarationList& select_list) {
	unordered_map<string, size_t> result;
	unordered_map<size_t, DeclarationList> group_to_selected_declarations;
	DeclarationList selected_declarations;
	unordered_set<string> unvisited_nodes;
	unordered_map<string, Declaration> selected_nodes;
	size_t group_number = 0;
	bool is_selected = false;

	for (auto& node : nodes) {
		unvisited_nodes.insert(node.first);
	}

	for (auto& select : select_list) {
		selected_nodes.insert({select.symbol, select});
	}

	queue<string> queue;
	if (!selected_nodes.empty()) {
		queue.push(selected_nodes.begin()->first);
		is_selected = true;
	} else {
		queue.push(*unvisited_nodes.begin());
	}

	while (!queue.empty()) {
		string symbol = queue.front();
		result.insert({symbol, group_number});
		if (selected_nodes.find(symbol) != selected_nodes.end()) {
			selected_declarations.push_back(selected_nodes[symbol]);
			selected_nodes.erase(symbol);
		}
		unvisited_nodes.erase(symbol);
		queue.pop();

		Node node = this->nodes.at(symbol);
		for (const string& adjacent_symbol : node.adjacent_symbols) {
			if (unvisited_nodes.find(adjacent_symbol) != unvisited_nodes.end()) {
				queue.push(adjacent_symbol);
			}
		}
		
		if (queue.empty() && !unvisited_nodes.empty()) {
			group_to_selected_declarations.insert({group_number, selected_declarations});
			selected_declarations.clear();
			if (!selected_nodes.empty()) {
				queue.push(selected_nodes.begin()->first);
			} else {
				queue.push(*unvisited_nodes.begin());
				is_selected = false;
			}
			group_number++;
		}
	}

	group_to_selected_declarations.insert({group_number, selected_declarations});
	return {
		group_number + 1,
		result,
		group_to_selected_declarations,
	};
}
