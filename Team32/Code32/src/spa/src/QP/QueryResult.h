#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

#include "Common/TypeDefs.h"

class QueryResult {
public:
	QueryResult();
	QueryResult(StmtRefList stmtList, VarRefList varList);
	StmtRefList getStmtRefList();
	VarRefList getVarRefList();

private:
	StmtRefList stmtList;
	VarRefList varList;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
