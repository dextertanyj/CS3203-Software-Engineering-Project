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
		if (patternClause.entRef.type == EntRefType::synonym) {
			addEdge(patternClause.synonym.symbol, patternClause.entRef.entRef);
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

unordered_set<string> QueryGraph::getNonTrivialSynonyms(string selectedSynonym) {
	// Run BFS on the selected node
	unordered_set<string> result;
	std::queue<string> queue;
	queue.push(selectedSynonym);
	
	while (!queue.empty()) {
		string symbol = queue.front();
		result.insert(symbol);
		queue.pop();
		
		Node node = this->nodes.at(symbol);
		for (string adjacentSymbol : node.adjacentSymbols) {
			if (result.find(adjacentSymbol) == result.end()) {
				queue.push(adjacentSymbol);
			}
		}
	}

	return result;
}
