#ifndef SPA_SRC_PKB_SETSTORE_H
#define SPA_SRC_PKB_SETSTORE_H

#include <unordered_set>

#include "PKB/PKB.h"

template <typename T>
class PKB::SetStore {
public:
	SetStore();
	void insert(const T& item);
	void insert(const std::unordered_set<T>& items);
	[[nodiscard]] std::unordered_set<T> getAll() const;

private:
	std::unordered_set<T> store;
};

#endif  // SPA_SRC_PKB_SETSTORE_H
