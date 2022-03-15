#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

#include <memory>
#include <string>
#include <unordered_map>

#include "QP/ConnectedSynonyms.h"
#include "QP/QueryProperties.h"
#include "QP/Relationship/Relation.h"

using namespace std;
using QP::Types::ClauseList;
using QP::Types::ConnectedSynonyms;
using QP::Types::DeclarationList;
using QP::Types::Node;

class QP::QueryGraph {
public:
	explicit QueryGraph(const DeclarationList& declarations);
	void setEdges(const ClauseList& clause_list);
	unordered_map<string, Node> getNodes();
	ConnectedSynonyms getConnectedSynonyms(const DeclarationList& select_list);

private:
	unordered_map<string, Node> nodes;
	void setEdge(const shared_ptr<Relationship::Relation>& relation);
	void addEdge(const pair<string, string>& symbols);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
