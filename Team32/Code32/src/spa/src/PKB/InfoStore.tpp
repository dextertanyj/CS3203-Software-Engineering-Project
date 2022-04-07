#ifndef SPA_SRC_PKB_INFOSTORE_TPP
#define SPA_SRC_PKB_INFOSTORE_TPP

#include "PKB/InfoStore.h"

#include <stdexcept>

template <typename TIdent, typename TContent, class TInfo>
PKB::InfoStore<TIdent, TContent, TInfo>::InfoStore() = default;

template <typename TIdent, typename TContent, class TInfo>
void PKB::InfoStore<TIdent, TContent, TInfo>::insert(const TIdent& identifier, TContent content) {
	assert(store.find(identifier) == store.end());
	shared_ptr<TInfo> info = shared_ptr<TInfo>(new TInfo(identifier, std::move(content)));
	store.emplace(identifier, std::move(info));
}

template <typename TIdent, typename TContent, class TInfo>
unordered_set<shared_ptr<TInfo>> PKB::InfoStore<TIdent, TContent, TInfo>::getAll() const {
	unordered_set<shared_ptr<TInfo>> result;
	for (const auto& key_value : store) {
		result.emplace(key_value.second);
	}
	return result;
}

template <typename TIdent, typename TContent, class TInfo>
shared_ptr<TInfo> PKB::InfoStore<TIdent, TContent, TInfo>::get(const TIdent& identifier) const {
	// Test the identifier by constructing a new TInfo object.
	TInfo test = TInfo(identifier, TContent());

	auto iter = store.find(identifier);
	if (iter == store.end()) {
		return nullptr;
	}
	return iter->second;
}

template <typename TIdent, typename TContent, class TInfo>
void PKB::InfoStore<TIdent, TContent, TInfo>::clear() {
	store.clear();
}

#endif  // SPA_SRC_PKB_INFOSTORE_TPP