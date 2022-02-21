#ifndef SPA_CONSTANTSTORE_H
#define SPA_CONSTANTSTORE_H

#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

using namespace std;

class PKB::ConstantStore {
public:
	ConstantStore();
	void insert(ConstVal value);
	void insert(const unordered_set<ConstVal>& values);
	unordered_set<ConstVal> getAll();

private:
	unordered_set<ConstVal> store;
};

#endif  // SPA_CONSTANTSTORE_H
