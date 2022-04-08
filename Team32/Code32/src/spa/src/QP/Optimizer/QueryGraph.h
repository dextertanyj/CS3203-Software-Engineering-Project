#ifndef SPA_SRC_QP_QUERYGRAPH_H
#define SPA_SRC_QP_QUERYGRAPH_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "QP/Evaluator/Clause.h"
#include "QP/Optimizer/ClauseGroups.h"
#include "QP/Optimizer/Optimizer.h"
#include "QP/QueryProperties.h"
#include "QP/Types.h"

class QP::Optimizer::QueryGraph {
public:
	struct Edge {
		std::string node_from_symbol;
		std::string node_to_symbol;
		std::shared_ptr<Evaluator::Clause> clause;
		size_t weight;
	};

	struct Node {
		std::string declaration_symbol;
		std::unordered_set<std::string> adjacent_symbols;
		std::vector<Edge> outgoing_edges;
		size_t weight;
	};

	explicit QueryGraph(const Types::DeclarationList& declarations, const Types::ClauseList& clauses,
	                    const Types::DeclarationList& select_list);
	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] std::vector<std::string> getGroupSynonyms(size_t group_number) const;
	[[nodiscard]] Types::DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] Types::ClauseList getGroupClauses(size_t group_number) const;
	[[nodiscard]] std::unordered_map<std::string, Node> getNodes() const;

private:
	struct EdgeComparator {
		[[nodiscard]] bool operator()(Edge& edge_one, Edge& edge_two) const { return edge_one.weight > edge_two.weight; };
	};

	std::unordered_map<std::string, Node> nodes;
	ClauseGroups clause_groups;

	void setEdges(const Types::ClauseList& clause_list);
	void setEdge(const std::shared_ptr<Evaluator::Clause>& clause);
	void addEdge(const std::pair<std::string, std::string>& symbols, const std::shared_ptr<Evaluator::Clause>& clause);
	void optimize(const Types::DeclarationList& select_list);
	void traverseGraph(std::unordered_set<std::string>& unvisited_nodes,
	                   std::unordered_map<std::string, Types::Declaration>& selected_nodes);

	void insertEdgesToQueue(std::unordered_set<std::string>& visited_nodes, const std::string& node_symbol,
	                        std::priority_queue<Edge, std::vector<Edge>, EdgeComparator>& pq) const;
	[[nodiscard]] std::string getCheapestNodeInGroup(size_t group_number) const;
	static void addNodeToQueue(const Node& node, std::queue<std::string>& queue, std::unordered_set<std::string>& unvisited_nodes,
	                           unsigned long long& cost);
};

#endif  // SPA_SRC_QP_QUERYGRAPH_H
