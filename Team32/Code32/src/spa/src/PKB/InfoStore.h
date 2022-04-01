#ifndef SPA_SRC_PKB_INFOSTORE_H
#define SPA_SRC_PKB_INFOSTORE_H

#include <unordered_map>

#include "PKB/PKB.h"

using namespace std;

template <typename TIdent, typename TContent, class TInfo>
class PKB::InfoStore {
public:
	InfoStore();
	void insert(const TIdent& identifier, TContent content);
	[[nodiscard]] shared_ptr<TInfo> get(const TIdent& identifier) const;
	[[nodiscard]] unordered_set<shared_ptr<TInfo>> getAll() const;
	void clear();

private:
	unordered_map<TIdent, shared_ptr<TInfo>> store;
};

#endif  // SPA_SRC_PKB_INFOSTORE_H
