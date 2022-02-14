#pragma once

#include "PKB/FollowsPKB.h"
#include "PKB/ParentPKB.h"
#include "PKB/StatementRelationStore.h"

using namespace std;

template <class T>
StatementRelationStore<T>::StatementRelationStore() = default;

template <class T>
void StatementRelationStore<T>::set(shared_ptr<StmtInfo> front, shared_ptr<StmtInfo> back) {
	StmtRef front_idx = front->reference;
	StmtRef back_idx = back->reference;

	if (front_idx >= back_idx) {
		throw invalid_argument("Second statement must come after the first statement");
	}

	auto front_iter = map.find(front_idx);
	if (front_iter == map.end()) {
		T relation = T(front);
		relation.insertReverse(back);
		map.insert(make_pair(front_idx, relation));
	} else {
		front_iter->second.insertReverse(back);
	}

	auto back_iter = map.find(back_idx);
	if (back_iter == map.end()) {
		T relation = T(back);
		relation.insertForward(front);
		map.insert({back_idx, relation});
	} else {
		back_iter->second.insertForward(front);
	}
}

template <class T>
bool StatementRelationStore<T>::isRelated(StmtRef front, StmtRef back) {
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
	return (*result.begin())->reference == back;
}

template <class T>
unordered_set<shared_ptr<StmtInfo>> StatementRelationStore<T>::getForward(StmtRef index) {
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
unordered_set<shared_ptr<StmtInfo>> StatementRelationStore<T>::getReverse(StmtRef index) {
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
unordered_set<shared_ptr<StmtInfo>> StatementRelationStore<T>::getForwardTransitive(StmtRef index) {
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
unordered_set<shared_ptr<StmtInfo>> StatementRelationStore<T>::getReverseTransitive(StmtRef index) {
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
void StatementRelationStore<T>::clear() {
	map.clear();
}