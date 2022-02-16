#ifndef SPA_PARENTRELATION_H
#define SPA_PARENTRELATION_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/StatementRelationStore.h"

class ParentRelation {
public:
	explicit ParentRelation(shared_ptr<StmtInfo> self);
	void insertForward(shared_ptr<StmtInfo> parent);
	void insertReverse(const shared_ptr<StmtInfo>& child);
	void appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> parents);
	void appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> children);

	unordered_set<shared_ptr<StmtInfo>> getForward();
	unordered_set<shared_ptr<StmtInfo>> getReverse();
	unordered_set<shared_ptr<StmtInfo>> getForwardTransitive();
	unordered_set<shared_ptr<StmtInfo>> getReverseTransitive();
	static void optimize(StatementRelationStore<ParentRelation>& store);

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> parent;
	unordered_set<shared_ptr<StmtInfo>> children;
	unordered_set<shared_ptr<StmtInfo>> parent_transitive;
	unordered_set<shared_ptr<StmtInfo>> children_transitive;
	static unordered_set<shared_ptr<StmtInfo>> populateTransitive(StatementRelationStore<ParentRelation>& store, ParentRelation& current,
	                                                              unordered_set<shared_ptr<StmtInfo>> previous);
};

#endif  // SPA_PARENTRELATION_H
