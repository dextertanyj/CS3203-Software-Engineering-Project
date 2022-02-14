#ifndef SPA_PARENTPKB_H
#define SPA_PARENTPKB_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/StatementRelationStore.h"

class ParentPKB {
public:
	explicit ParentPKB(shared_ptr<StmtInfo> self);
	void insertForward(shared_ptr<StmtInfo> parent);
	void insertReverse(const shared_ptr<StmtInfo>& child);
	void appendForwardTransitive(unordered_set<shared_ptr<StmtInfo>> parents);
	void appendReverseTransitive(unordered_set<shared_ptr<StmtInfo>> children);

	unordered_set<shared_ptr<StmtInfo>> getForward();
	unordered_set<shared_ptr<StmtInfo>> getReverse();
	unordered_set<shared_ptr<StmtInfo>> getForwardTransitive();
	unordered_set<shared_ptr<StmtInfo>> getReverseTransitive();
	static void optimize(StatementRelationStore<ParentPKB>& store);

private:
	shared_ptr<StmtInfo> self;
	shared_ptr<StmtInfo> parent;
	unordered_set<shared_ptr<StmtInfo>> children;
	unordered_set<shared_ptr<StmtInfo>> parent_transitive;
	unordered_set<shared_ptr<StmtInfo>> children_transitive;
	static unordered_set<shared_ptr<StmtInfo>> populateTransitive(StatementRelationStore<ParentPKB>& store, ParentPKB& current,
	                                                              unordered_set<shared_ptr<StmtInfo>> previous);
};

#endif  // SPA_PARENTPKB_H
