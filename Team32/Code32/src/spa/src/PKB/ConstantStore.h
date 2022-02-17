#ifndef SPA_CONSTANTSTORE_H
#define SPA_CONSTANTSTORE_H

#include <unordered_set>
#include "Common/TypeDefs.h"

using namespace std;

class ConstantStore {
public:
	ConstantStore();
	void insert(ConstVal value);
	void insert(const unordered_set<ConstVal>& values);
	unordered_set<ConstVal> getAll();

private:
	unordered_set<ConstVal> store;
};

#endif  // SPA_CONSTANTSTORE_H
