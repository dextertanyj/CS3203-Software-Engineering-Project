#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

#include <memory>
#include <string>
#include <unordered_map>

#include "QP/QueryProperties.h"
#include "QP/Relationship/Relation.h"

using namespace std;

class QP::QueryGraph {
public:
	explicit QueryGraph(const Types::DeclarationList& declarations);
	void setEdges(const Types::ClauseList& clause_list);
	unordered_map<string, Types::Node> getNodes();
	unordered_map<string, size_t> getSynonymsInGroup(const string& selected_synonym);

private:
	unordered_map<string, Types::Node> nodes;
	void setEdge(const shared_ptr<Relationship::Relation>& relation);
	void addEdge(const pair<string, string>& symbols);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
