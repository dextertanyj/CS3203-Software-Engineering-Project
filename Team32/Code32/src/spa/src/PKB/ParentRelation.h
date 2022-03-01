#ifndef SPA_SRC_PKB_PARENTRELATION_H
#define SPA_SRC_PKB_PARENTRELATION_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/StatementRelationStore.h"

class PKB::ParentRelation {
public:
	explicit ParentRelation(shared_ptr<StmtInfo> self);
	void insertForward(const shared_ptr<StmtInfo>& parent_to_insert);
	void insertReverse(const shared_ptr<StmtInfo>& child_to_insert);
	void appendForwardTransitive(StmtInfoPtrSet parents_to_insert);
	void appendReverseTransitive(StmtInfoPtrSet children_to_insert);

	StmtInfoPtrSet getForward();
	StmtInfoPtrSet getReverse();
	StmtInfoPtrSet getForwardTransitive();
	StmtInfoPtrSet getReverseTransitive();
	static void optimize(StatementRelationStore<ParentRelation>& store);

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> parent;
	StmtInfoPtrSet children;
	StmtInfoPtrSet parent_transitive;
	StmtInfoPtrSet children_transitive;
	static StmtInfoPtrSet populateTransitive(StatementRelationStore<ParentRelation>& store, ParentRelation& current,
	                                                              StmtInfoPtrSet previous);
};

#endif  // SPA_SRC_PKB_PARENTRELATION_H
