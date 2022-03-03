#ifndef SPA_SRC_PKB_SETSTORE_TPP
#define SPA_SRC_PKB_SETSTORE_TPP

#include "SetStore.h"

template <typename T>
PKB::SetStore<T>::SetStore() = default;

template <typename T>
void PKB::SetStore<T>::insert(const T& item) {
	store.insert(item);
}

template <typename T>
void PKB::SetStore<T>::insert(const unordered_set<T>& items) {
	store.insert(items.begin(), items.end());
}

template <typename T>
unordered_set<T> PKB::SetStore<T>::getAll() const {
	return store;
}

#endif  // SPA_SRC_PKB_SETSTORE_TPP