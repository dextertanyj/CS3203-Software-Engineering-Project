#include "PKB/ProcedureStore.h"

PKB::ProcedureStore::ProcedureStore() = default;

void PKB::ProcedureStore::insert(const ProcRef& procedure, vector<shared_ptr<StmtInfo>> statements) {
	if (procedure.empty()) {
		throw invalid_argument("Invalid procedure name.");
	}
	if (store.find(procedure) != store.end()) {
		throw logic_error("Procedure scope has already been defined.");
	}
	shared_ptr<ProcedureInfo> info = shared_ptr<ProcedureInfo>(new ProcedureInfo(procedure, std::move(statements)));
	store.insert({procedure, std::move(info)});
}

unordered_set<shared_ptr<PKB::ProcedureInfo>> PKB::ProcedureStore::getAllInfo() const {
	unordered_set<shared_ptr<PKB::ProcedureInfo>> result;
	for (const auto& key_value : store) {
		result.insert(key_value.second);
	}
	return result;
}

unordered_set<ProcRef> PKB::ProcedureStore::getAll() const {
	unordered_set<ProcRef> result;
	for (const auto& key_value : store) {
		result.insert(key_value.first);
	}
	return result;
}

shared_ptr<PKB::ProcedureInfo> PKB::ProcedureStore::get(const ProcRef& procedure) const {
	auto iter = store.find(procedure);
	if (iter == store.end()) {
		return nullptr;
	}
	return iter->second;
}

void PKB::ProcedureStore::clear() { store.clear(); }
