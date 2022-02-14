#include "StatementStore.h"

StatementStore::StatementStore() = default;

void StatementStore::insert(StmtRef idx, StmtType type) {
	if (idx <= 0) {
		throw "Statement index must be positive";
	}

	auto iter = store.find(idx);
	if (iter != store.end() && iter->second->type != type) {
		throw "Statement has already been assigned a different type";
	}
	if (iter != store.end()) {
		return;
	}
	shared_ptr<StmtInfo> info = make_shared<StmtInfo>(StmtInfo({idx, type}));
	this->store.insert({idx, info});
}

shared_ptr<StmtInfo> StatementStore::get(StmtRef idx) {
	if (idx <= 0) {
		throw "Statement index must be positive";
	}
	auto iter = this->store.find(idx);
	if (iter == store.end()) {
		return nullptr;
	}
	return iter->second;
}

unordered_set<shared_ptr<StmtInfo>> StatementStore::getAll() {
	unordered_set<shared_ptr<StmtInfo>> result;
	for (const auto& key_value : store) {
		result.insert(key_value.second);
	}
	return result;
}

void StatementStore::clear() { store.clear(); }