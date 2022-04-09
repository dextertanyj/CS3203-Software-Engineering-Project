#ifndef SPA_SRC_PKB_INFOSTORE_TPP
#define SPA_SRC_PKB_INFOSTORE_TPP

#include "PKB/InfoStore.h"

#include "PKB/PKB.h"

template <typename TIdent, typename TContent, class TInfo>
PKB::InfoStore<TIdent, TContent, TInfo>::InfoStore() = default;

template <typename TIdent, typename TContent, class TInfo>
void PKB::InfoStore<TIdent, TContent, TInfo>::insert(const TIdent& identifier, TContent content) {
	// Not an assertion since procedures have possibly duplicate names.
	if (store.find(identifier) != store.end()) {
		throw DuplicateEntityException("Duplicate entity detected.");
	}
	std::shared_ptr<TInfo> info = std::shared_ptr<TInfo>(new TInfo(identifier, std::move(content)));
	store.emplace(identifier, std::move(info));
}

template <typename TIdent, typename TContent, class TInfo>
std::shared_ptr<TInfo> PKB::InfoStore<TIdent, TContent, TInfo>::get(const TIdent& identifier) const {
	auto iter = store.find(identifier);
	if (iter == store.end()) {
		return nullptr;
	}
	return iter->second;
}

template <typename TIdent, typename TContent, class TInfo>
std::unordered_set<std::shared_ptr<TInfo>> PKB::InfoStore<TIdent, TContent, TInfo>::getAll() const {
	std::unordered_set<std::shared_ptr<TInfo>> result;
	for (const auto& key_value : store) {
		result.emplace(key_value.second);
	}
	return result;
}

template <typename TIdent, typename TContent, class TInfo>
void PKB::InfoStore<TIdent, TContent, TInfo>::clear() {
	store.clear();
}

#endif  // SPA_SRC_PKB_INFOSTORE_TPP