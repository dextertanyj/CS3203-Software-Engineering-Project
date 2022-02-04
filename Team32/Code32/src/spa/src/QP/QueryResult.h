#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

#include "Common/TypeDefs.h"

class QueryResult {
public:
	QueryResult();
	QueryResult(StmtInfoList stmtList, VarRefList varList);
	StmtInfoList getStmtInfoList();
	VarRefList getVarRefList();

private:
	StmtInfoList stmtList;
	VarRefList varList;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
