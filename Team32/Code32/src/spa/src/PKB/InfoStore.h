#ifndef SPA_SRC_PKB_INFOSTORE_H
#define SPA_SRC_PKB_INFOSTORE_H

#include <unordered_map>

#include "PKB/PKB.h"

template <typename TIdent, typename TContent, class TInfo>
class PKB::InfoStore {
public:
	InfoStore();
	void insert(const TIdent& identifier, TContent content);
	[[nodiscard]] std::shared_ptr<TInfo> get(const TIdent& identifier) const;
	[[nodiscard]] std::unordered_set<std::shared_ptr<TInfo>> getAll() const;
	void clear();

private:
	std::unordered_map<TIdent, std::shared_ptr<TInfo>> store;
};

#endif  // SPA_SRC_PKB_INFOSTORE_H
