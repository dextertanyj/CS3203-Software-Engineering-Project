#include "QueryResult.h"

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
