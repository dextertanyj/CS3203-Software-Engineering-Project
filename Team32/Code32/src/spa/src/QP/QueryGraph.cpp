#include "QueryGraph.h"

#include <queue>

QueryGraph::QueryGraph(const DeclarationList& declarations) {
	for (Declaration declaration : declarations) {
		Node node = { declaration.symbol, {} };
		nodes.insert({ declaration.symbol, node });
	}
}

void QueryGraph::setEdges(const SuchThatClauseList& suchThatClauseList, const PatternClauseList& patternClauseList) {
	for (SuchThatClause suchThatClause : suchThatClauseList) {
		vector<string> declarations = suchThatClause.relation->getDeclarationSymbols();
		if (declarations.size() == 2) {
			addEdge(declarations[0], declarations[1]);
		}
	}

	for (PatternClause patternClause : patternClauseList) {
		if (patternClause.queryPattern->getEntRef().type == EntRefType::synonym) {
			addEdge(patternClause.queryPattern->getSynonym().symbol, patternClause.queryPattern->getEntRef().entRef);
		}
	}
}

unordered_map<string, Node> QueryGraph::getNodes() {
	return nodes;
}

void QueryGraph::addEdge(string symbolOne, string symbolTwo) {
	Node nodeOne = this->nodes.at(symbolOne);
	if (find(nodeOne.adjacentSymbols.begin(), nodeOne.adjacentSymbols.end(), symbolTwo) ==
			nodeOne.adjacentSymbols.end()) {
		nodeOne.adjacentSymbols.push_back(symbolTwo);
		nodes[symbolOne] = nodeOne;
	}

	Node nodeTwo = this->nodes.at(symbolTwo);
	if (find(nodeTwo.adjacentSymbols.begin(), nodeTwo.adjacentSymbols.end(), symbolOne) ==
			nodeTwo.adjacentSymbols.end()) {
		nodeTwo.adjacentSymbols.push_back(symbolOne);
		nodes[symbolTwo] = nodeTwo;
	}
}

vector<unordered_set<string>> QueryGraph::getSynonymsInGroup(string selectedSynonym) {
	// Run BFS on the selected node
	vector<unordered_set<string>> result;
	unordered_set<string> group;
	unordered_set<string> unvisitedNodes;

	for (auto &node : nodes) {
		unvisitedNodes.insert(node.first);
	}

	queue<string> queue;
	queue.push(selectedSynonym);
	
	while (!queue.empty()) {
		string symbol = queue.front();
		group.insert(symbol);
		unvisitedNodes.erase(symbol);
		queue.pop();
		
		Node node = this->nodes.at(symbol);
		for (string adjacentSymbol : node.adjacentSymbols) {
			if (unvisitedNodes.find(adjacentSymbol) != unvisitedNodes.end()) {
				queue.push(adjacentSymbol);
			}
		}

		// If queue is empty but there are still unvisited nodes,
		// we add an unvisited node to queue
		if (queue.empty() && !unvisitedNodes.empty()) {
			queue.push(*unvisitedNodes.begin());
			result.push_back(group);
			group.clear();
		}
	}

	result.push_back(group);
	return result;
}
