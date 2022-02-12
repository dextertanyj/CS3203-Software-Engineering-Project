#include "UseStore.h"

using namespace std;

UseStore::UseStore() {}

void UseStore::setUses(shared_ptr<StmtInfo> stmt_info, VarRef var_name) {
    StmtRef stmt_num = stmt_info->reference;
	if (stmt_num <= 0) throw invalid_argument("Statement number must be a positive integer.");
	if (var_name.length() == 0) throw invalid_argument("Variable name length cannot be 0.");

    auto key_var_to_stmt_map = varToStmtMap.find(var_name);
	auto key_stmt_to_var_map = stmtToVarMap.find(stmt_num);

	// Check if var-stmt mapping has already been set.
	if (key_stmt_to_var_map != stmtToVarMap.end() && key_stmt_to_var_map->second.find(var_name) != key_stmt_to_var_map->second.end() ) {
		throw invalid_argument("setUses() has already been done for this StmtInfo and VarRef.");
	}

	if (key_var_to_stmt_map == varToStmtMap.end()) {
        varToStmtMap.insert(make_pair(var_name, unordered_set<shared_ptr<StmtInfo>>{stmt_info}));
    } else {
		key_var_to_stmt_map->second.insert(stmt_info);
    }

    if (key_stmt_to_var_map == stmtToVarMap.end()) {
        stmtToVarMap.insert(make_pair(stmt_num, unordered_set<VarRef>{var_name}));
    } else {
		key_stmt_to_var_map->second.insert(var_name);
    }
}

// Checks if variable var_name is used at statement stmt_info.
bool UseStore::checkUses(shared_ptr<StmtInfo> stmt_info, VarRef var_name) {
    StmtRef stmt_num = stmt_info->reference;
    if (stmt_num <= 0) throw invalid_argument("Statement number must be a positive integer.");
	if (var_name.length() == 0) throw invalid_argument("Variable name length cannot be 0.");

    auto key_var_to_stmt_map = varToStmtMap.find(var_name);
	if (key_var_to_stmt_map == varToStmtMap.end()) return false;

    for (const auto& itr : key_var_to_stmt_map->second) {
        if (stmt_info == itr) {
            return true;
        }
    }
    return false;
}

bool UseStore::checkUsesList(vector<shared_ptr<StmtInfo>> stmtInfoList, VarRef varName) {
    for (auto& itr : stmtInfoList) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");
        if (checkUses(itr, varName)) return true;
    }
    return false;
}

unordered_set<shared_ptr<StmtInfo>> UseStore::getUsesByVar(VarRef var_name) {
	if (var_name.length() == 0) throw invalid_argument("Variable name length cannot be 0.");
	auto key_var_to_stmt_map = varToStmtMap.find(var_name);
    if (key_var_to_stmt_map == varToStmtMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>{};
    } else {
        return key_var_to_stmt_map->second;
    }
}

unordered_set<VarRef> UseStore::getUsesByStmt(shared_ptr<StmtInfo> stmtInfo) {
    if (stmtInfo == nullptr) throw invalid_argument("Provided statement info is a null pointer.");
	StmtRef stmtNo = stmtInfo->reference;
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = stmtToVarMap.find(stmtNo);
    if (keyItr == stmtToVarMap.end()) {
        return unordered_set<VarRef>{};
    } else {
        return keyItr->second;
    }
}

unordered_set<VarRef> UseStore::getUsesByStmtList(vector<shared_ptr<StmtInfo>> stmtInfoList) {
    unordered_set<VarRef> varSet;
    for (auto& itr : stmtInfoList) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");

        unordered_set<VarRef> stmtVarSet = getUsesByStmt(itr);
        varSet.insert(stmtVarSet.begin(), stmtVarSet.end());
    }
    return varSet;
}

void UseStore::clear() {
    varToStmtMap.clear();
    stmtToVarMap.clear();
}
