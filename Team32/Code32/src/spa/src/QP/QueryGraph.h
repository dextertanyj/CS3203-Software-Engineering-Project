#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include "QP/ConnectedSynonyms.h"
#include "QP/QueryProperties.h"
#include "QP/Relationship/Relation.h"

using namespace std;
using QP::Types::Clause;
using QP::Types::ClauseList;
using QP::Types::ConnectedSynonyms;
using QP::Types::DeclarationList;
using QP::Types::Edge;
using QP::Types::Node;

class QP::QueryGraph {
public:
	explicit QueryGraph(const DeclarationList& declarations);
	void setEdges(const ClauseList& clause_list);
	unordered_map<string, Node> getNodes();
	ConnectedSynonyms getConnectedSynonyms(const DeclarationList& select_list);
	ClauseList sortGroup(size_t group_number);

private:
	unordered_map<string, Node> nodes;
	ConnectedSynonyms connected_synonyms;
	void setEdge(const Clause& clause);
	void addEdge(const pair<string, string>& symbols, const Clause& clause);
	string getCheapestNodeInGroup(size_t group_number);
	void insertEdgesToQueue(unordered_set<string>& visited_nodes, string node_symbol,
	                        priority_queue<Edge, vector<Edge>, QP::Types::EdgeComp>& pq);
	static void addNodesToQueue(unordered_set<string>& symbols, queue<string>& queue, unordered_set<string>& unvisited_nodes);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
