#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "QP/QueryProperties.h"

using namespace std;

struct Node {
	string declarationSymbol;
	vector<string> adjacentSymbols;
};

class QueryGraph {
public:
	explicit QueryGraph(const DeclarationList& declarations);
	void setEdges(const SuchThatClauseList& suchThatClauseList, const PatternClauseList& patternClauseList);
	unordered_map<string, Node> getNodes();

private:
	unordered_map<string, Node> nodes;
	void addEdge(string symbolOne, string symbolTwo);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_
