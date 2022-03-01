#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

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
	void setEdge(const shared_ptr<Relationship::Relation>& relation);
	void addEdge(const pair<string, string>& symbols);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
