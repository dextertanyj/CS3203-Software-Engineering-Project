#include "QueryGraph.h"

QueryGraph::QueryGraph(DeclarationList& declarations) {
	for (Declaration declaration : declarations) {
		Node node = { declaration.symbol, {} };
		nodes.insert({ declaration.symbol, node });
	}
}

void QueryGraph::setEdges(SuchThatClauseList& suchThatClauseList, PatternClauseList& patternClauseList) {
	for (SuchThatClause suchThatClause : suchThatClauseList) {
		vector<string> declarations = suchThatClause.relation->getDeclarationSymbols();
		if (declarations.size()== 2) {
			addEdge(declarations[0], declarations[1]);
		}
	}

	for (PatternClause patternClause : patternClauseList) {
		if (patternClause.entRef.type == EntRefType::synonym) {
			addEdge(patternClause.synonym.symbol, patternClause.entRef.entRef);
		}
	}
}

void QueryGraph::addEdge(string symbolOne, string symbolTwo) {
	Node nodeOne = this->nodes.at(symbolOne);
	if (find(nodeOne.adjacentSymbols.begin(), nodeOne.adjacentSymbols.end(), symbolTwo) !=
			nodeOne.adjacentSymbols.end()) {
		nodeOne.adjacentSymbols.push_back(symbolTwo);
	}

	Node nodeTwo = this->nodes.at(symbolTwo);
	if (find(nodeTwo.adjacentSymbols.begin(), nodeTwo.adjacentSymbols.end(), symbolOne) !=
			nodeTwo.adjacentSymbols.end()) {
		nodeTwo.adjacentSymbols.push_back(symbolOne);
	}
}
