#include "QueryResult.h"

#include <algorithm>
#include <iterator>

QueryResult::QueryResult() {}

QueryResult::QueryResult(StmtRefList stmtList, VarRefList varList)
	: stmtList(stmtList),
	  varList(varList) {}

StmtRefList QueryResult::getStmtRefList() {
	return stmtList;
}

VarRefList QueryResult::getVarRefList() {
	return varList;
}

bool QueryResult::updateStmtList(StmtRefList newStmtList) {
	StmtRefList result;
	std::sort(this->stmtList.begin(), this->stmtList.end());
	std::sort(newStmtList.begin(), newStmtList.end());
	
	std::set_intersection(
		this->stmtList.begin(), this->stmtList.end(),
		newStmtList.begin(), newStmtList.end(),
		back_inserter(result));

	this->stmtList = result;

	return !result.empty();
}

bool QueryResult::updateVarList(VarRefList newVarList) {
	VarRefList result;
	std::sort(this->varList.begin(), this->varList.end());
	std::sort(newVarList.begin(), newVarList.end());

	std::set_intersection(
		this->varList.begin(), this->varList.end(),
		newVarList.begin(), newVarList.end(),
		back_inserter(result));

	this->varList = result;

	return !result.empty();
}
