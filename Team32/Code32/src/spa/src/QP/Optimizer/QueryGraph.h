#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include "QP/Evaluator/Clause.h"
#include "QP/Optimizer/ClauseGroups.h"
#include "QP/Optimizer/Optimizer.h"
#include "QP/QueryProperties.h"

class QP::Optimizer::QueryGraph {
public:
	struct Edge {
		string node_from_symbol;
		string node_to_symbol;
		shared_ptr<Evaluator::Clause> clause;
		size_t weight;
	};

	struct Node {
		string declaration_symbol;
		unordered_set<string> adjacent_symbols;
		vector<Edge> outgoing_edges;
		size_t weight;
	};

	explicit QueryGraph(const Types::DeclarationList& declarations, const Types::ClauseList& clauses,
	                    const Types::DeclarationList& select_list);
	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] vector<string> getGroupSynonyms(size_t group_number) const;
	[[nodiscard]] Types::DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] Types::ClauseList getGroupClauses(size_t group_number) const;
	[[nodiscard]] unordered_map<string, Node> getNodes() const;

private:
	struct EdgeComparator {
		auto operator()(Edge& edge_one, Edge& edge_two) const -> bool { return edge_one.weight > edge_two.weight; };
	};

	unordered_map<string, Node> nodes;
	ClauseGroups clause_groups;

	void setEdges(const Types::ClauseList& clause_list);
	void setEdge(const shared_ptr<Evaluator::Clause>& clause);
	void addEdge(const pair<string, string>& symbols, const shared_ptr<Evaluator::Clause>& clause);
	void optimize(const Types::DeclarationList& select_list);

	void insertEdgesToQueue(unordered_set<string>& visited_nodes, const string& node_symbol,
	                        priority_queue<Edge, vector<Edge>, EdgeComparator>& pq) const;
	[[nodiscard]] string getCheapestNodeInGroup(size_t group_number) const;
	static void addNodeToQueue(const Node& node, queue<string>& queue, unordered_set<string>& unvisited_nodes, unsigned long long& cost);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
