#include "ModifyStore.h"

using namespace std;

ModifyStore::ModifyStore() {}

void ModifyStore::setModify(shared_ptr<StmtInfo> stmt_info, VarRef var_name) {
    StmtRef stmt_num = stmt_info->reference;
	if (stmt_num <= 0) throw invalid_argument("Statement number must be a positive integer.");
	if (var_name.length() == 0) throw invalid_argument("Variable name must have length more than 0.");

    auto key_var_to_stmt_map = varToStmtMap.find(var_name);
	auto key_stmt_to_var_map = stmtToVarMap.find(stmt_num);

	if (key_stmt_to_var_map == stmtToVarMap.end()) {
		stmtToVarMap.insert(make_pair(stmt_num, var_name));
	} else {
		// One-to-one stmt-modify relation already stored. Abort.
		throw invalid_argument("Modifies relationship already stored for this statement.");
	}

    if (key_var_to_stmt_map == varToStmtMap.end()) {
        varToStmtMap.insert(make_pair(var_name, unordered_set<shared_ptr<StmtInfo>>{stmt_info}));
    } else {
		key_var_to_stmt_map->second.insert(stmt_info);
    }
}

bool ModifyStore::checkModifies(shared_ptr<StmtInfo> stmt_info, VarRef var_name) {
    StmtRef stmt_num = stmt_info->reference;
    if (stmt_num <= 0) throw invalid_argument("Statement number must be a positive integer.");
	if (var_name.length() == 0) throw invalid_argument("Variable name must have length more than 0.");

    auto key_var_to_stmt_map = varToStmtMap.find(var_name);
    if (key_var_to_stmt_map == varToStmtMap.end()) return false;

    for (auto& itr : key_var_to_stmt_map->second) {
        if (stmt_info == itr) {
            return true;
        }
    }
    return false;
}

bool ModifyStore::checkModifiesList(vector<shared_ptr<StmtInfo>> stmtInfoList, VarRef varName) {
    for (auto& itr : stmtInfoList) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");
        if (checkModifies(itr, varName)) return true;
    }
    return false;
}

// Should return only one modifies
VarRef ModifyStore::getModifiesByStmt(shared_ptr<StmtInfo> stmt_info) {
    StmtRef stmt_num = stmt_info->reference;
    if (stmt_num <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto key_stmt_to_var_map = stmtToVarMap.find(stmt_num);
    if (key_stmt_to_var_map == stmtToVarMap.end()) {
		throw invalid_argument("This modifies relation is not stored - did you do setModify()?");
	}
	return key_stmt_to_var_map->second;
}

unordered_set<shared_ptr<StmtInfo>> ModifyStore::getModifiesByVar(VarRef varName) {
    auto keyItr = varToStmtMap.find(varName);
    if (keyItr == varToStmtMap.end()) {
        return unordered_set<shared_ptr<StmtInfo>>{};
    } else {
        return keyItr->second;
    }
}

unordered_set<VarRef> ModifyStore::getModifiesByStmtList(vector<shared_ptr<StmtInfo>> stmt_info_list) {
    unordered_set<VarRef> var_set;
    for (auto& itr : stmt_info_list) {
        if (itr->reference <= 0) throw invalid_argument("Statement number must be a positive integer.");
        VarRef stmt_var = getModifiesByStmt(itr);
		var_set.insert(stmt_var);
    }
    return var_set;
}

void ModifyStore::clear() {
    varToStmtMap.clear();
    stmtToVarMap.clear();
}

