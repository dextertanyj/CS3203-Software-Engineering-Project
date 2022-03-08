#ifndef SPA_NODECOMPARATOR_H
#define SPA_NODECOMPARATOR_H

#include "PKB/PKB.h"

class NodeComparator {
public:
	bool operator()(pair<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>> n1,
	                pair<StmtRef, unordered_set<shared_ptr<PKB::NodeInfo>>> n2) {
		return n1.first > n2.first;
	}
};
#endif  // SPA_NODECOMPARATOR_H
