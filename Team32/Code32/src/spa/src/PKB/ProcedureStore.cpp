#include "ProcedureStore.h"

ProcedureStore::ProcedureStore() = default;

void ProcedureStore::setProc(const ProcRef& proc_name, const vector<shared_ptr<StmtInfo>>& idx_list) {
	for (const auto& itr : idx_list) {
		StmtRef stmt_no = itr->reference;
		if (stmt_no <= 0) {
			throw invalid_argument("Statement number must be a positive integer.");
		}

		auto key_itr1 = stmt_to_proc_map.find(stmt_no);
		if (key_itr1 != stmt_to_proc_map.end()) {
			throw invalid_argument("Statement is already part of a procedure.");
		}
		stmt_to_proc_map.insert(make_pair(stmt_no, proc_name));
	}

	auto key_itr = proc_map.find(proc_name);
	if (key_itr == proc_map.end()) {
		ProcRelation proc_relation = {idx_list, nullptr};
		proc_map.insert(make_pair(proc_name, proc_relation));
	} else if (key_itr->second.idx_list.empty()) {
		key_itr->second.idx_list = idx_list;
	} else {
		throw invalid_argument("Procedure already exists in Procedure Map.");
	}
}

void ProcedureStore::setCall(const shared_ptr<StmtInfo>& call_stmt, const ProcRef& proc_name) {
	if (call_stmt->reference <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	if (call_stmt->type != StmtType::Call) {
		throw invalid_argument("Statement type must be a call statement.");
	}

	auto key_itr = proc_map.find(proc_name);
	if (key_itr == proc_map.end()) {
		ProcRelation proc_relation = {vector<shared_ptr<StmtInfo>>(), call_stmt};
		proc_map.insert(make_pair(proc_name, proc_relation));
	} else if (key_itr->second.call_stmt == nullptr) {
		key_itr->second.call_stmt = call_stmt;
	} else {
		throw invalid_argument("Procedure already has a call statement.");
	}
}

vector<shared_ptr<StmtInfo>> ProcedureStore::getStmtsByProc(const ProcRef& proc_name) {
	auto key_itr = proc_map.find(proc_name);
	if (key_itr == proc_map.end()) {
		return vector<shared_ptr<StmtInfo>>{};
	}
	return key_itr->second.idx_list;
}

ProcRef ProcedureStore::getProcByCall(const shared_ptr<StmtInfo>& call_stmt) {
	if (call_stmt->reference <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	if (call_stmt->type != StmtType::Call) {
		throw invalid_argument("Statement type must be a call statement.");
	}

	for (auto& itr : proc_map) {
		if (itr.second.call_stmt == call_stmt) {
			return itr.first;
		}
	}
	return "";
}

ProcRef ProcedureStore::getProcByStmt(const shared_ptr<StmtInfo>& stmt_info) {
	StmtRef stmt_no = stmt_info->reference;
	if (stmt_no <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto key_itr = stmt_to_proc_map.find(stmt_no);
	if (key_itr == stmt_to_proc_map.end()) {
		return "";
	}
	return key_itr->second;
}

unordered_set<ProcRef> ProcedureStore::getProcListByStmtList(const unordered_set<shared_ptr<StmtInfo>>& stmt_info_list) {
	unordered_set<ProcRef> proc_set;
	for (const auto& itr : stmt_info_list) {
		if (itr->reference <= 0) {
			throw invalid_argument("Statement number must be a positive integer.");
		}

		ProcRef proc_name = getProcByStmt(itr);
		if (!proc_name.empty()) {
			proc_set.insert(proc_name);
		}
	}
	return proc_set;
};

void ProcedureStore::clear() { proc_map.clear(); }
