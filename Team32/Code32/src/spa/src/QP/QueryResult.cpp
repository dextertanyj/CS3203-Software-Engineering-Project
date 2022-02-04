#include "QueryResult.h"

QueryResult::QueryResult() {}

QueryResult::QueryResult(StmtInfoList stmtList, VarRefList varList)
	: stmtList(stmtList),
	  varList(varList) {}

StmtInfoList QueryResult::getStmtInfoList() {
	return stmtList;
}

VarRefList QueryResult::getVarRefList() {
	return varList;
}
