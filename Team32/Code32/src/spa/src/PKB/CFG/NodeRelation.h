#ifndef SPA_NODERELATION_H
#define SPA_NODERELATION_H
#include <memory>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.h"

class PKB::NodeRelation {
public:
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

	void setCFGIndex(StmtRef ref);

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

template <>
unordered_set<shared_ptr<PKB::NodeInfo>> PKB::TransitiveRelationStore<StmtRef, PKB::NodeInfo, PKB::NodeRelation>::populateTransitive(
	NodeRelation& current, unordered_set<shared_ptr<NodeInfo>> next_nodes_transitive);

#endif  // SPA_NODERELATION_H
