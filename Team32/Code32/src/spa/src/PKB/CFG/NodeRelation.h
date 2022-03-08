#ifndef SPA_NODERELATION_H
#define SPA_NODERELATION_H
#include <memory>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.h"

class PKB::NodeRelation {
public:
	explicit NodeRelation(shared_ptr<StmtInfo> self);
	void insertForward(const shared_ptr<StmtInfo>& previous_node);
	void insertReverse(const shared_ptr<StmtInfo>& next_node);
	void appendForwardTransitive(StmtInfoPtrSet new_previous_nodes);
	void appendReverseTransitive(StmtInfoPtrSet new_child_nodes);

	[[nodiscard]] shared_ptr<StmtInfo> getSelf() const;
	[[nodiscard]] StmtInfoPtrSet getForward() const;
	[[nodiscard]] StmtInfoPtrSet getReverse() const;
	[[nodiscard]] StmtInfoPtrSet getForwardTransitive() const;
	[[nodiscard]] StmtInfoPtrSet getReverseTransitive() const;

	void setLastNode();
	void setCFGIndex(StmtRef ref);

private:
	shared_ptr<StmtInfo> self;
	StmtInfoPtrSet previous_nodes;
	StmtInfoPtrSet next_nodes;
	StmtInfoPtrSet previous_nodes_transitive;
	StmtInfoPtrSet next_nodes_transitive;
	bool is_last_node;
	StmtRef cfg_index;
};

// Template specializations for CFG Node relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::NodeRelation>::optimize();

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::NodeRelation>::populateTransitive(
	NodeRelation& current, StmtInfoPtrSet previous);

#endif  // SPA_NODERELATION_H
