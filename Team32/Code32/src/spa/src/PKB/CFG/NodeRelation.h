#ifndef SPA_NODERELATION_H
#define SPA_NODERELATION_H
#include <memory>
#include <queue>
#include <set>
#include <stdexcept>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "DummyNodeInfo.h"
#include "NodeComparator.h"
#include "NodeInfo.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.h"
#include "PKB/Types.h"

class PKB::NodeRelation {
public:
	NodeRelation() = default;
	explicit NodeRelation(shared_ptr<NodeInfo> self);
	void insertForward(const shared_ptr<NodeInfo>& previous_node);
	void insertReverse(const shared_ptr<NodeInfo>& next_node);
	void appendForwardTransitive(unordered_set<shared_ptr<NodeInfo>> new_previous_nodes);
	void appendReverseTransitive(unordered_set<shared_ptr<NodeInfo>> new_next_nodes);

	[[nodiscard]] shared_ptr<NodeInfo> getSelf() const;
	[[nodiscard]] unordered_set<shared_ptr<NodeInfo>> getForward() const;
	[[nodiscard]] unordered_set<shared_ptr<NodeInfo>> getReverse() const;
	[[nodiscard]] unordered_set<shared_ptr<NodeInfo>> getForwardTransitive() const;
	[[nodiscard]] unordered_set<shared_ptr<NodeInfo>> getReverseTransitive() const;
	[[nodiscard]] static shared_ptr<PKB::NodeInfo> findLastNode(
		shared_ptr<NodeInfo> node_ref, PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& store);
	void setCFGIndex(StmtRef ref);
	void removeForward(shared_ptr<NodeInfo> node_info);
	void removeReverse(shared_ptr<NodeInfo> node_info);
	static void populateUniqueIndex(const NodeRelation& node_relation, StmtRef stmt_no,
	                                PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>& store);

private:
	shared_ptr<NodeInfo> self;
	unordered_set<shared_ptr<NodeInfo>> previous_nodes;
	unordered_set<shared_ptr<NodeInfo>> next_nodes;
	unordered_set<shared_ptr<NodeInfo>> previous_nodes_transitive;
	unordered_set<shared_ptr<NodeInfo>> next_nodes_transitive;
};

// Template specializations for CFG Node relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>::optimize();

#endif  // SPA_NODERELATION_H
