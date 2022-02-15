#ifndef SPA_STATEMENTSTORE_H
#define SPA_STATEMENTSTORE_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"

using namespace std;

class StatementStore {
public:
	StatementStore();
	void insert(StmtRef idx, StmtType type);
	shared_ptr<StmtInfo> get(StmtRef idx);
	unordered_set<shared_ptr<StmtInfo>> getAll();
	void clear();

private:
	unordered_map<StmtRef, shared_ptr<StmtInfo>> store;
};

#endif  // SPA_STATEMENTSTORE_H
