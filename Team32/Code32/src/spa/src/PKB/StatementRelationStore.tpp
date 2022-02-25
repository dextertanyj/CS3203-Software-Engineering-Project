#ifndef SPA_SRC_PKB_STATEMENTRELATIONSTORE_TPP
#define SPA_SRC_PKB_STATEMENTRELATIONSTORE_TPP

#include "PKB/StatementRelationStore.h"

#include "PKB/FollowsRelation.h"
#include "PKB/ParentRelation.h"

using namespace std;

template <class T>
PKB::StatementRelationStore<T>::StatementRelationStore() = default;

template <class T>
void PKB::StatementRelationStore<T>::set(shared_ptr<StmtInfo> front,
                                         shared_ptr<StmtInfo> back) {  // NOLINT(bugprone-easily-swappable-parameters)
	StmtRef front_idx = front->reference;
	StmtRef back_idx = back->reference;

	if (front_idx >= back_idx) {
		throw invalid_argument("Second statement must come after the first statement");
	}

	auto front_iter = map.find(front_idx);
	auto back_iter = map.find(back_idx);

	if (back_iter != map.end() && !(back_iter->second).getForward().empty()) {
		throw invalid_argument("Back statement already has a forward statement");
	}

	if (front_iter == map.end()) {
		T relation = T(front);
		relation.insertReverse(back);
		map.insert(make_pair(front_idx, relation));
	} else {
		front_iter->second.insertReverse(back);
	}

	if (back_iter == map.end()) {
		T relation = T(back);
		relation.insertForward(front);
		map.insert({back_idx, relation});
	} else {
		back_iter->second.insertForward(front);
	}
}

template <class T>
bool PKB::StatementRelationStore<T>::isRelated(StmtRef front, StmtRef back) {
	if (front <= 0 || back <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	if (front >= back) {
		return false;
	}

	auto iter = map.find(front);
	if (iter == map.end()) {
		return false;
	}
	auto result = iter->second.getReverse();
	if (result.empty()) {
		return false;
	}
	return (any_of(result.begin(), result.end(), [back](const shared_ptr<StmtInfo>& ptr) { return ptr->reference == back; }));
}

template <class T>
unordered_set<shared_ptr<StmtInfo>> PKB::StatementRelationStore<T>::getForward(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = map.find(index);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getForward();
}

template <class T>
unordered_set<shared_ptr<StmtInfo>> PKB::StatementRelationStore<T>::getReverse(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	auto iter = map.find(index);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getReverse();
}

template <class T>
unordered_set<shared_ptr<StmtInfo>> PKB::StatementRelationStore<T>::getForwardTransitive(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto iter = map.find(index);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getForwardTransitive();
}

template <class T>
unordered_set<shared_ptr<StmtInfo>> PKB::StatementRelationStore<T>::getReverseTransitive(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto iter = map.find(index);
	if (iter == map.end()) {
		return {};
	}
	return iter->second.getReverseTransitive();
}

template <class T>
void PKB::StatementRelationStore<T>::clear() {
	map.clear();
}

#endif  // SPA_SRC_PKB_STATEMENTRELATIONSTORE_TPP
