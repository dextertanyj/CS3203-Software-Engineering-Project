#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "QP/QueryProperties.h"

using namespace std;

class QP::QueryGraph {
public:
	explicit QueryGraph(const DeclarationList& declarations);
	void setEdges(const SuchThatClauseList& such_that_clause_list, const PatternClauseList& pattern_clause_list);
	unordered_map<string, Node> getNodes();

	/*
	 * The first group will contain the selected synonym
	 */
	vector<unordered_set<string>> getSynonymsInGroup(const string& selected_synonym);

private:
	unordered_map<string, Node> nodes;
	void addEdge(const string& symbol_one, const string& symbol_two);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYGRAPH_H_
