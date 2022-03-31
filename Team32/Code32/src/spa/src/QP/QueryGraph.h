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

typedef struct Edge {
	string node_from_symbol;
	string node_to_symbol;
	Clause clause;
	size_t weight;
} Edge;

struct EdgeComp {
	auto operator()(Edge& edge_one, Edge& edge_two) const -> bool { return edge_one.weight > edge_two.weight; };
};

typedef struct Node {
	string declaration_symbol;
	unordered_set<string> adjacent_symbols;
	vector<Edge> outgoing_edges;
	size_t weight;
} Node;

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
	void insertEdgesToQueue(unordered_set<string>& visited_nodes, const string& node_symbol,
	                        priority_queue<Edge, vector<Edge>, EdgeComp>& pq);
	static void addNodeToQueue(const Node& node, queue<string>& queue, unordered_set<string>& unvisited_nodes,
	                            unsigned long long& cost);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
