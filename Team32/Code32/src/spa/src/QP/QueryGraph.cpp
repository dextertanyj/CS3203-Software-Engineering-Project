#include "QueryGraph.h"

#include <queue>
#include <vector>

using QP::Types::Clause;
using QP::Types::ConnectedSynonyms;
using QP::Types::Declaration;

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
	return connected_synonyms;
}
