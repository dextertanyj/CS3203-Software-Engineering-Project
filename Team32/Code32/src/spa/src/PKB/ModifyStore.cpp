#include "ModifyStore.h"

using namespace std;

ModifyStore::ModifyStore() {}

void ModifyStore::setModify(shared_ptr<StmtInfo> statement, VarRef variable) {
	StmtRef index = statement->reference;
	if (variable.length() == 0) {
		throw invalid_argument("Variable name must have length more than 0.");
	}

	auto variable_iter = varToStmtMap.find(variable);
	auto statement_iter = stmtToVarMap.find(index);

	if (statement_iter == stmtToVarMap.end()) {
		stmtToVarMap.insert({index, {variable}});
	} else {
		statement_iter->second.insert(variable);
	}

	if (variable_iter == varToStmtMap.end()) {
		varToStmtMap.insert({variable, {statement}});
	} else {
		variable_iter->second.insert(statement);
	}
}

void ModifyStore::setModify(shared_ptr<StmtInfo> statement, unordered_set<VarRef> variables) {
	StmtRef index = statement->reference;
	for (const VarRef& variable : variables) {
		if (variable.length() == 0) {
			throw invalid_argument("Variable name must have length more than 0.");
		}
	}
	auto statement_iter = stmtToVarMap.find(index);
	if (statement_iter == stmtToVarMap.end()) {
		stmtToVarMap.insert({index, variables});
	} else {
		statement_iter->second.insert(variables.begin(), variables.end());
	}

	for (const VarRef& variable : variables) {
		auto variable_iter = varToStmtMap.find(variable);
		if (variable_iter == varToStmtMap.end()) {
			varToStmtMap.insert({variable, {statement}});
		} else {
			variable_iter->second.insert(statement);
		}
	}
}

bool ModifyStore::checkModifies(StmtRef index, VarRef variable) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}
	if (variable.length() == 0) {
		throw invalid_argument("Variable name must have length more than 0.");
	}

	auto iter = varToStmtMap.find(variable);
	if (iter == varToStmtMap.end()) {
		return false;
	}

	return any_of(iter->second.begin(), iter->second.end(),
	              [index](const shared_ptr<StmtInfo>& statement) { return statement->reference == index; });
}

unordered_set<VarRef> ModifyStore::getModifiesByStmt(StmtRef index) {
	if (index <= 0) {
		throw invalid_argument("Statement number must be a positive integer.");
	}

	auto iter = stmtToVarMap.find(index);
	if (iter == stmtToVarMap.end()) {
		return {};
	}
	return iter->second;
}

unordered_set<shared_ptr<StmtInfo>> ModifyStore::getModifiesByVar(VarRef variable) {
	if (variable.length() == 0) {
		throw invalid_argument("Variable name must have length more than 0.");
	}

	auto iter = varToStmtMap.find(variable);
	if (iter == varToStmtMap.end()) {
		return {};
	}
	return iter->second;
}

void ModifyStore::optimize(unordered_map<StmtRef, shared_ptr<StmtInfo>> statement_store, ParentStore& parent_store) {
	for (auto statement : statement_store) {
		if (statement.second->type == StmtType::IfStmt || statement.second->type == StmtType::WhileStmt) {
			auto children = parent_store.getChildStar(statement.second->reference);
			unordered_set<VarRef> variables;
			for (auto child : children) {
				auto iter = stmtToVarMap.find(child->reference);
				if (iter != stmtToVarMap.end()) {
					variables.insert(iter->second.begin(), iter->second.end());
				}
			}
			this->setModify(statement.second, variables);
		}
	}
}

void ModifyStore::clear() {
	varToStmtMap.clear();
	stmtToVarMap.clear();
}
