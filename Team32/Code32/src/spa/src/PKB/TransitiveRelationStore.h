#ifndef SPA_SRC_PKB_TRANSITIVERELATIONSTORE_H
#define SPA_SRC_PKB_TRANSITIVERELATIONSTORE_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

template <typename TIdent, typename TInfo, class TRelation>
class PKB::TransitiveRelationStore {
public:
	TransitiveRelationStore();
	void set(shared_ptr<TInfo> front, shared_ptr<TInfo> back);
	[[nodiscard]] bool isRelated(TIdent front, TIdent back) const;
	[[nodiscard]] bool isTransitivelyRelated(TIdent front, TIdent back) const;

	[[nodiscard]] unordered_set<shared_ptr<TInfo>> getForward(TIdent ident) const;
	[[nodiscard]] unordered_set<shared_ptr<TInfo>> getReverse(TIdent ident) const;
	[[nodiscard]] unordered_set<shared_ptr<TInfo>> getForwardTransitive(TIdent ident) const;
	[[nodiscard]] unordered_set<shared_ptr<TInfo>> getReverseTransitive(TIdent ident) const;

	void optimize();
	void clear();

private:
	unordered_map<TIdent, TRelation> map;
	// NOLINTNEXTLINE(modernize-use-equals-delete)
	unordered_set<shared_ptr<TInfo>> populateTransitive(TRelation& current, unordered_set<shared_ptr<TInfo>> previous) = delete;
};

#endif  // SPA_SRC_PKB_TRANSITIVERELATIONSTORE_H
