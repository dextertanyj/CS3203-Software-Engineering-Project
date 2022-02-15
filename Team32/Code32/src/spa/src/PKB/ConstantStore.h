#ifndef SPA_CONSTANTSTORE_H
#define SPA_CONSTANTSTORE_H

#include <unordered_set>

using namespace std;

class ConstantStore {
public:
	ConstantStore();
	void insert(int value);
	void insert(const unordered_set<int>& values);
	unordered_set<int> getAll();

private:
	unordered_set<int> store;
};

#endif  // SPA_CONSTANTSTORE_H
