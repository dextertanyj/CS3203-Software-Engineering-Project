#ifndef SPA_SRC_PKB_PARENTRELATION_H
#define SPA_SRC_PKB_PARENTRELATION_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/StatementInfo.h"
#include "PKB/TransitiveRelationStore.h"

class PKB::ParentRelation {
public:
	explicit ParentRelation(shared_ptr<StmtInfo> self);
	void insertForward(const shared_ptr<StmtInfo>& parent_to_insert);
	void insertReverse(const shared_ptr<StmtInfo>& child_to_insert);
	void appendForwardTransitive(StmtInfoPtrSet parents_to_insert);
	void appendReverseTransitive(StmtInfoPtrSet children_to_insert);

	[[nodiscard]] shared_ptr<StmtInfo> getSelf() const;
	[[nodiscard]] StmtInfoPtrSet getForward() const;
	[[nodiscard]] StmtInfoPtrSet getReverse() const;
	[[nodiscard]] StmtInfoPtrSet getForwardTransitive() const;
	[[nodiscard]] StmtInfoPtrSet getReverseTransitive() const;

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> parent;
	StmtInfoPtrSet children;
	StmtInfoPtrSet parent_transitive;
	StmtInfoPtrSet children_transitive;
};

// Template specializations for Parent relationship.

template <>
void PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::ParentRelation>::optimize();

template <>
StmtInfoPtrSet PKB::TransitiveRelationStore<StmtRef, StmtInfo, PKB::ParentRelation>::populateTransitive(ParentRelation& current,
                                                                                                        StmtInfoPtrSet previous);

#endif  // SPA_SRC_PKB_PARENTRELATION_H
