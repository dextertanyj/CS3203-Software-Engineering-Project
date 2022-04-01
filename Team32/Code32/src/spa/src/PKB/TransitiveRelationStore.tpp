#ifndef SPA_SRC_PKB_TRANSITIVERELATIONSTORE_TPP
#define SPA_SRC_PKB_TRANSITIVERELATIONSTORE_TPP

#include "PKB/TransitiveRelationStore.h"

#include <stdexcept>

template <typename TIdent, typename TInfo, class TRelation>
PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::TransitiveRelationStore() = default;

template <typename TIdent, typename TInfo, class TRelation>
void PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::set(shared_ptr<TInfo> front,
                                                                 shared_ptr<TInfo> back) {  // NOLINT(bugprone-easily-swappable-parameters)
	TIdent front_ident = front->getIdentifier();
	TIdent back_ident = back->getIdentifier();

	auto front_iter = map.find(front_ident);
	auto back_iter = map.find(back_ident);

	// Attempt to insert forward facing relationship first since they tend to have more restrictions.
	if (back_iter == map.end()) {
		TRelation relation = TRelation(back);
		relation.insertForward(front);
		map.insert({back_ident, relation});
	} else {
		back_iter->second.insertForward(front);
	}

	if (front_iter == map.end()) {
		TRelation relation = TRelation(front);
		relation.insertReverse(back);
		map.insert(make_pair(front_ident, relation));
	} else {
		front_iter->second.insertReverse(back);
	}
}

template <typename TIdent, typename TInfo, class TRelation>
bool PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::isRelated(TIdent front, TIdent back) const {
	auto iter = map.find(front);
	if (iter == map.end()) {
		return false;
	}
	auto result = iter->second.getReverse();
	if (result.empty()) {
		return false;
	}
	return (any_of(result.begin(), result.end(), [back](const shared_ptr<TInfo>& ptr) { return ptr->getIdentifier() == back; }));
}

template <typename TIdent, typename TInfo, class TRelation>
bool PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::isTransitivelyRelated(TIdent front, TIdent back) const {
	auto iter = map.find(front);
	if (iter == map.end()) {
		return false;
	}
	auto result = iter->second.getReverseTransitive();
	if (result.empty()) {
		return false;
	}
	return (any_of(result.begin(), result.end(), [back](const shared_ptr<TInfo>& ptr) { return ptr->getIdentifier() == back; }));
}

template <typename TIdent, typename TInfo, class TRelation>
unordered_set<shared_ptr<TInfo>> PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::getForward(TIdent ident) const {
	auto iter = map.find(ident);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getForward();
}

template <typename TIdent, typename TInfo, class TRelation>
unordered_set<shared_ptr<TInfo>> PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::getReverse(TIdent ident) const {
	auto iter = map.find(ident);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getReverse();
}

template <typename TIdent, typename TInfo, class TRelation>
unordered_set<shared_ptr<TInfo>> PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::getForwardTransitive(TIdent ident) const {
	auto iter = map.find(ident);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getForwardTransitive();
}

template <typename TIdent, typename TInfo, class TRelation>
unordered_set<shared_ptr<TInfo>> PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::getReverseTransitive(TIdent ident) const {
	auto iter = map.find(ident);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getReverseTransitive();
}

template <typename TIdent, typename TInfo, class TRelation>
void PKB::TransitiveRelationStore<TIdent, TInfo, TRelation>::clear() {
	map.clear();
}

#endif  // SPA_SRC_PKB_TRANSITIVERELATIONSTORE_TPP
