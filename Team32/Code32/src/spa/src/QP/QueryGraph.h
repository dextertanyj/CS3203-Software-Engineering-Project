#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include "QP/ClauseGroups.h"
#include "QP/QueryProperties.h"
#include "QP/Relationship/Relation.h"

using namespace std;
using QP::Types::Clause;
using QP::Types::ClauseGroups;
using QP::Types::ClauseList;
using QP::Types::DeclarationList;

class QP::QueryGraph {
public:
	struct Edge {
		string node_from_symbol;
		string node_to_symbol;
		Clause clause;
		size_t weight;
	};

	struct Node {
		string declaration_symbol;
		unordered_set<string> adjacent_symbols;
		vector<Edge> outgoing_edges;
		size_t weight;
	};

	explicit QueryGraph(const DeclarationList& declarations, const ClauseList& clauses, const DeclarationList& select_list);
	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] vector<string> getGroupSynonyms(size_t group_number) const;
	[[nodiscard]] DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] ClauseList getGroupClauses(size_t group_number) const;
	[[nodiscard]] unordered_map<string, Node> getNodes() const;

private:
	struct EdgeComparator {
		auto operator()(Edge& edge_one, Edge& edge_two) const -> bool { return edge_one.weight > edge_two.weight; };
	};

	unordered_map<string, Node> nodes;
	ClauseGroups connected_synonyms;

	void setEdges(const ClauseList& clause_list);
	void setEdge(const Clause& clause);
	void addEdge(const pair<string, string>& symbols, const Clause& clause);
	void optimize(const DeclarationList& select_list);

	void insertEdgesToQueue(unordered_set<string>& visited_nodes, const string& node_symbol,
	                        priority_queue<Edge, vector<Edge>, EdgeComparator>& pq) const;
	string getCheapestNodeInGroup(size_t group_number) const;
	static void addNodeToQueue(const Node& node, queue<string>& queue, unordered_set<string>& unvisited_nodes, unsigned long long& cost);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
