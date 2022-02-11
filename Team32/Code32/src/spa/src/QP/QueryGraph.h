#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypeDefs.h"
#include "QP/Relationship//Relation.h"

using namespace std;

struct Node {
	string declarationSymbol;
	vector<string> adjacentSymbols;
};

class QueryGraph {
public:
	explicit QueryGraph(DeclarationList& declarations);
	void setEdges(SuchThatClauseList& suchThatClauseList, PatternClauseList& patternClauseList);
	void addEdge(string symbolOne, string symbolTwo);

private:
	unordered_map<string, Node> nodes;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_
