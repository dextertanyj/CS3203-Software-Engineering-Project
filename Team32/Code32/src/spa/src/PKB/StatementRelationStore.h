#ifndef SPA_STATEMENTRELATIONSTORE_H
#define SPA_STATEMENTRELATIONSTORE_H

#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"

template <class T>
class StatementRelationStore {
public:
	StatementRelationStore();
	void set(shared_ptr<StmtInfo>, shared_ptr<StmtInfo>);
	bool isRelated(StmtRef, StmtRef);

	unordered_set<shared_ptr<StmtInfo>> getForward(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getReverse(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getForwardTransitive(StmtRef);
	unordered_set<shared_ptr<StmtInfo>> getReverseTransitive(StmtRef);
	void clear();

private:
	unordered_map<StmtRef, T> map;

	friend class ParentRelation;
	friend class FollowsRelation;
};

#endif  // SPA_STATEMENTRELATIONSTORE_H
