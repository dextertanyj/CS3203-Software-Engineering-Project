#include "QueryResult.h"

#include "Common/TypeDefs.h"

QueryResult::QueryResult() {}

QueryResult::QueryResult(StmtInfoList stmtList, VarRefList varList):
	stmtList(stmtList), 
	varList(varList) {
}
